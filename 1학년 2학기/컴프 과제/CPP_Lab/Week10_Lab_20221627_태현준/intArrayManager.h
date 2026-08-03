class IntArrayManager{
public:
    IntArrayManager ();
    IntArrayManager ( int len, int *arr );
    ~IntArrayManager ();
    void concatenate ( IntArrayManager &y );
    bool compare ( IntArrayManager &y );
    void print();

private:
    int *arr;
    int length;
};
