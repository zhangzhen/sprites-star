#include <cmath>
#include <ctime>
#include <algorithm>
#include <getopt.h>
#include <sstream>
#include <iterator>
#include <queue>

#include "error.h"
#include "Deletion.h"
#include "ClipReader.h"
#include "BamStatCalculator.h"
#include "Helper.h"
//#include "Parameters.h"
#include "clip.h"
#include "range.h"
#include "Thirdparty/Timer.h"

#include "easylogging++.h"

#include "globals.h"
#include "BamToolsRGToLibMapReader.h"
#include "BamToolsLibInsertSizeEstimator.h"
#include "MaxDistDiffBiPartitioner.h"
#include "AnovaBiPartitionQualifier.h"
#include "MedianPositionPicker.h"
#include "CompositeTargetRegionFinder.h"
#include "TargetRegionToLeftFinder.h"
#include "TargetRegionToRightFinder.h"
#include "BamToolsPairsToLeftReader.h"
#include "BamToolsPairsToRightReader.h"
#include "PerChromDeletionFinder.h"

//
// Getopt
//
#define PROGRAM_NAME "sprites"
#define PROGRAM_VERSION "1.0"
#define PROGRAM_BUGREPORT "zhangz@csu.edu.cn"
const int DEFAULT_MIN_OVERLAP=12;
const int DEFAULT_MIN_MAPQUAL=1;
const int DEFAULT_SD_CUTOFF=4;

static const char *DFINDER_VERSION_MESSAGE =
PROGRAM_NAME " Version " PROGRAM_VERSION "\n"
"Written by Zhen Zhang.\n"
"\n"
"Copyright 2013 netlab.csu.edu.cn\n";

static const char *DFINDER_USAGE_MESSAGE =
"Usage: " PROGRAM_NAME " [OPTION] ... BAMFILE\n"
"Find deletions from records in BAMFILE\n"
"\n"
"      --help                           display this help and exit\n"
"      -v, --verbose                    display verbose output\n"
"      -r, --reffile=FILE               read the reference sequence from FILE\n"
"      -o, --outfile=FILE               write the deletion calls to FILE (default: BAMFILE.calls)\n"
"      -e, --error-rate=F               the maximum error rate allowed between two sequences to consider them overlapped (default: 0.04)\n"
"      -m, --min-overlap=LEN            minimum overlap required between two reads (default: 12)\n"
"      -q, --mapping-qual=MAPQ          minimum mapping quality of a read (default: 1)\n"
"      -n, --allowed-num=SIZE           a soft-clip is defined as valid, when the clipped part is not less than SIZE (default: 5)\n"
"\nThe following two option must appear together (if ommitted, attempt ot learn the mean and the standard deviation of insert size):\n"
"      -i, --insert-mean=N              the mean of insert size\n"
"          --enhanced-mode              enable the enhanced mode, in which reads of type 2 are considered besides type 1\n"
"      -s, --insert-sd=N                the standard deviation of insert size\n"
"\nReport bugs to " PROGRAM_BUGREPORT "\n\n";

namespace opt
{
    static unsigned int verbose;
    static std::string bamFile;
    static std::string refFile;
    static std::string outFile;
    static double errorRate = 0.04;
    static int minOverlap = DEFAULT_MIN_OVERLAP;
    static int minMapQual = DEFAULT_MIN_MAPQUAL;
    static int allowedNum = 12;
    static int mode = 0;

    static bool bLearnInsert = true;
    static int insertMean;
    static int insertSd;
}

static const char* shortopts = "o:q:r:e:m:n:i:s:v";

enum { OPT_HELP = 1, OPT_VERSION, OPT_ENHANCED_MODE };

static const struct option longopts[] = {
    { "verbose",        no_argument,       NULL, 'v' },
    { "min-overlap",    required_argument, NULL, 'm' },
    { "mapping-qual",   required_argument, NULL, 'q' },
    { "allowed-num",    required_argument, NULL, 'n' },
    { "reffile",        required_argument, NULL, 'r'},
    { "outfile",        required_argument, NULL, 'o' },
    { "error-rate",     required_argument, NULL, 'e' },
    { "insert-mean",    required_argument, NULL, 'i' },
    { "insert-sd",      required_argument, NULL, 's' },
    { "help",           no_argument,       NULL, OPT_HELP },
    { "version",        no_argument,       NULL, OPT_VERSION },
    { "enhanced-mode",  no_argument,       NULL, OPT_ENHANCED_MODE },
    { NULL, 0, NULL, 0 }
};

void parseOptions(int argc, char** argv);
void output(const std::string& filename, const std::vector<Deletion>& dels);

_INITIALIZE_EASYLOGGINGPP

void MakeLibraries(IReadGroupToLibraryMapReader *pMapReader, std::map<std::string, Library*> &libraries)
{
	std::map<std::string, std::string> readGroupToLibMap;
	if (pMapReader->GetMap(readGroupToLibMap))
	{
		for (auto &elt : readGroupToLibMap)
		{
			if (libraries.count(elt.second))
			{
				libraries[elt.second]->AddReadGroup(elt.first);
			}
			else
			{
				libraries[elt.second] = new Library(elt.second, elt.first);
			}
		}
		return;
	}

	libraries[NORGTAGLIBNAME] = new Library(NORGTAGLIBNAME, NORGTAGREADGROUPNAME);
}

void EstimateInsertSizeForLibraries(ILibraryInsertSizeEstimator *pEstimator, std::map<std::string, Library*> &libraries)
{
	for (auto &p : libraries)
	{
		pEstimator->estimate(p.second);
	}
}

void FindTargetRegions(PerChromDeletionFinder &finder, std::ostream& out)
{
	std::vector<TargetRegion *> regions;
	finder.FindTargetRegions(regions);
	for (auto &pRegion : regions)
	{
		out << *pRegion << std::endl;
	}
	finder.Clear();
}

void FindVariants(PerChromDeletionFinder &finder, const CallParams &cParams, std::ofstream& out, size_t &i)
{
	std::vector<Deletion*> variants;
	finder.FindCalls(cParams, variants);
	auto finalVariants = finder.MergeCalls(variants);
	for (auto &pVariant : finalVariants)
	{
		out << *pVariant << "\tDEL." << i + 1 << "." << pVariant->getFromTag() << std::endl;
		i++;
	}
	finder.Clear();
}

//
// Main
//
int main(int argc, char *argv[]) {

    parseOptions(argc, argv);

    if (opt::bLearnInsert) {
        std::cout << "Estimate the mean and standard deviation of insert size:" << std::endl;
        BamStatCalculator calc(opt::bamFile);
        opt::insertMean = calc.getInsertMean();
        opt::insertSd = calc.getInsertSd();
        std::cout << "Mean: " << opt::insertMean << std::endl;
        std::cout << "Sd: " << opt::insertSd << std::endl;
    }

//    Parameters params = { opt::allowedNum,
//                          opt::mode,
//                          opt::minOverlap,
//                          1.0f - opt::errorRate,
//                          opt::insertMean,
//                          opt::insertSd };
	BamTools::BamReader bamReader;
	if (!bamReader.Open(opt::bamFile))
		error("Could not open the input BAM file.");
	if (!bamReader.LocateIndex())
		error("Could not locate the index file");

	IReadGroupToLibraryMapReader *pMapReader = new BamToolsRGToLibMapReader(&bamReader);
	std::map<std::string, Library*> libraries;
	MakeLibraries(pMapReader, libraries);
	ILibraryInsertSizeEstimator *pEstimator = new BamToolsLibInsertSizeEstimator(&bamReader);
	EstimateInsertSizeForLibraries(pEstimator, libraries);

	CompositeTargetRegionFinder tRegionToLeftFinder;
	CompositeTargetRegionFinder tRegionToRightFinder;

	IBiPartitioner* pPartitioner = new MaxDistDiffBiPartitioner();
	IBiPartitionQualifier* pQualifier = new AnovaBiPartitionQualifier(0.0005);
	IPositionPicker* pPosPicker = new MedianPositionPicker();

	for (auto elt : libraries)
	{
		ISpanningPairsReader *pPairsToLeftReader = new BamToolsPairsToLeftReader(elt.second, &bamReader);
		tRegionToLeftFinder.Add(new TargetRegionToLeftFinder(pPairsToLeftReader,
			pPartitioner,
			pQualifier,
			pPosPicker));
		ISpanningPairsReader *pPairsToRightReader = new BamToolsPairsToRightReader(elt.second, &bamReader);
		tRegionToRightFinder.Add(new TargetRegionToRightFinder(pPairsToRightReader,
			pPartitioner,
			pQualifier,
			pPosPicker));
	}

    ClipReader creader(opt::bamFile, opt::allowedNum, opt::mode, opt::minMapQual, opt::insertMean + DEFAULT_SD_CUTOFF * opt::insertSd);


    FaidxWrapper faidx(opt::refFile);

    //int insLength = opt::insertMean + 3 * opt::insertSd;
    //double identityRate = 1.0f - opt::errorRate;

	int prevId = -1;
	int currentId;

	size_t i = 0;
	CallParams cParams(opt::minOverlap, opt::errorRate);
	PerChromDeletionFinder finder(&tRegionToLeftFinder, &tRegionToRightFinder, &faidx);

	std::ofstream out(opt::outFile.c_str());

	Timer* pTimer = new Timer("Calling deletions");
	AbstractClip *pRead;

	while ((pRead = creader.nextClip()))
	{
		currentId = pRead->getReferenceId();
		//        std::cout << currentId << std::endl;
		if (prevId != -1 && prevId != currentId)
		{
			//            FindTargetRegions(finder, out);
			FindVariants(finder, cParams, out, i);
		}

		finder.AddRead(pRead);
		prevId = currentId;
	}
	//FindTargetRegions(finder, std::cout);
	FindVariants(finder, cParams, out, i);

	delete pTimer;

    if (!i) {
        std::cout << "No deletion was found." << std::endl;
        return 0;
    }

    //output(opt::outFile, finalDels);

    return 0;
}

//
// Handle command line arguments
//
void parseOptions(int argc, char** argv)
{
    bool bInsertMean = false;
    bool bInsertSd = false;
    bool die = false;
    for (char c; (c = getopt_long(argc, argv, shortopts, longopts, NULL)) != -1;)
    {
        std::istringstream arg(optarg != NULL ? optarg : "");
        switch (c)
        {
            case 'n': arg >> opt::allowedNum; break;
            case 'm': arg >> opt::minOverlap; break;
            case 'q': arg >> opt::minMapQual; break;
            case 'r': arg >> opt::refFile; break;
            case 'o': arg >> opt::outFile; break;
            case 'e': arg >> opt::errorRate; break;
            case '?': die = true; break;
            case 'v': opt::verbose++; break;
            case 'i': arg >> opt::insertMean; bInsertMean = true; break;
            case 's': arg >> opt::insertSd; bInsertSd = true; break;
            case OPT_ENHANCED_MODE: opt::mode = 1; break;
            case OPT_HELP:
                std::cout << DFINDER_USAGE_MESSAGE;
                exit(EXIT_SUCCESS);
            case OPT_VERSION:
                std::cout << DFINDER_VERSION_MESSAGE;
                exit(EXIT_SUCCESS);
        }
    }

    if (argc - optind < 1)
    {
        std::cerr << PROGRAM_NAME ": missing arguments\n";
        die = true;
    }
    else if (argc - optind > 1)
    {
        std::cerr << PROGRAM_NAME ": too many arguments\n";
        die = true;
    }

    if (bInsertMean & bInsertSd) {
        opt::bLearnInsert = false;
    }

    if (bInsertMean ^ bInsertSd) {
        std::cerr << PROGRAM_NAME ": the mean and standard deviation of insert size must be specified together\n";
        die = true;
    }

    if(opt::errorRate > 1.0f)
    {
        std::cerr << PROGRAM_NAME ": invalid error-rate parameter: " << opt::errorRate << "\n";
        die = true;
    }

    if(opt::refFile.empty())
    {
        std::cerr << PROGRAM_NAME ": the reference file must be specified\n";
        die = true;
    }

    if (die)
    {
        std::cout << "\n" << DFINDER_USAGE_MESSAGE;
        exit(EXIT_FAILURE);
    }

    // Validate parameters
    if(opt::errorRate <= 0)
        opt::errorRate = 0.0f;

    // Parse the input filename
    opt::bamFile = argv[optind++];

    std::string out_prefix = stripFilename(opt::bamFile);
    if(opt::outFile.empty())
    {
        opt::outFile = out_prefix + ".bedpe";
    }

}

void output(const std::string &filename, const std::vector<Deletion> &dels) {
    std::ofstream out(filename.c_str());
    size_t i = 1;
    std::for_each(std::begin(dels), std::end(dels), [&i, &out](const Deletion &d)
    {out << d << "\tDEL." << i << "." << d.getFromTag() << std::endl; i++;});
}
