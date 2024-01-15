#ifndef ERROR_H
#define ERROR_H

// error/status codes defined here

// add error codes under appropriate heading
// -- remember to add corresponding message to
//    Error.print() also!

enum Status
{

    // no error

    OK = 0,
    NOTUSED1 = -999,

    // Wname and Passwords errors
    SIZERROR,
    BADINPUT,

    // BufMgr and HashTable errors

    HASHTBLERROR,
    HASHNOTFOUND,
    BUFFEREXCEEDED,
    PAGENOTPINNED,
    BADBUFFER,
    PAGEPINNED,

    // Page errors

    NOSPACE,
    NORECORDS,
    ENDOFPAGE,
    BADPAGEPTR,
    // B+ Tree errors
    BADINDEXPARM,
    TUPLENOTFOUND,
    NULLROOT,
    NOTFOUND,
    NOSIBLINGS,
    NULLPTR,

    // Index errors

    RECNOTFOUND,
    BUCKETFULL,
    DIROVERFLOW,
    NONUNIQUEENTRY,
    NOMORERECS,
    INDEXNOTFOUND,

    // Catalog errors

    BADCATPARM,
    RELNOTFOUND,
    ATTRNOTFOUND,
    NAMETOOLONG,
    DUPLATTR,
    RELEXISTS,
    NOINDEX,
    INDEXEXISTS,
    ATTRTOOLONG,

    // Utility errors

    // Query errors

    ATTRTYPEMISMATCH,
    TMP_RES_EXISTS,

    // do not touch filler -- add codes before it

    NOTUSED2
};

// definition of Error class

class Error
{
  public:
    void print(Status status);
};

#define ASSERT(c)                                                              \
    {                                                                          \
        if (!(c))                                                              \
        {                                                                      \
            cerr << "At line " << __LINE__ << ":" << endl << "  ";             \
            cerr << "This condition should hold: " #c << endl;                 \
            exit(1);                                                           \
        }                                                                      \
    }

#endif
