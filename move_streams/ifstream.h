#pragma once
#include <string>
#include <cstdio>

struct ifstream
{
    ifstream();
    explicit ifstream(const char *path);
    ifstream(const ifstream &src);
    ifstream(ifstream &&src);
    ifstream& operator=(ifstream src);
    ~ifstream();

    void swap(ifstream &other);
    bool good() const;
    bool fail() const;
    bool eof() const;
private:
    FILE *file_;
    const char *path_;

    friend ifstream& operator>>(ifstream &in, int &out);
    friend ifstream& operator>>(ifstream &in, double &out);
    friend ifstream& operator>>(ifstream &in, std::string &out);
};

void swap(ifstream &first, ifstream &second);
