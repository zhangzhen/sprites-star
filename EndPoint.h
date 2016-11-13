#ifndef ENDPOINT
#define ENDPOINT


class EndPoint
{
public:
    EndPoint(int x, size_t ownerIndex, bool left)
        : x(x),
          ownerIndex(ownerIndex),
          left(left)
    {}

    int GetX() const { return x; }
    size_t GetOwnerIndex() const { return ownerIndex; }
    bool IsLeft() const { return left; }

    bool operator < (const EndPoint &other) const
    {
        return x < other.x;
    }

private:
    int x;
    size_t ownerIndex;
    bool left;
};

#endif // ENDPOINT

