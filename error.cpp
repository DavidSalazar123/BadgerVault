#include <iostream>
#include <sys/types.h>
using namespace std;
#include "error.h"

void Error::print(Status status)
{
    cerr << "Error: ";
    switch (status)
    {

    case OK:
        cerr << "no error";
        break;

    case SIZERROR:
        cerr << "Wname or Password doesn't follow size of 64 characters limit";
        break;
    case BADINPUT:
        cerr << "Bad input";
        break;

        // BufMgr and HashTable errors

    case HASHTBLERROR:
        cerr << "hash table error";
        break;
    case HASHNOTFOUND:
        cerr << "hash entry not found";
        break;
    case BUFFEREXCEEDED:
        cerr << "buffer pool full";
        break;
    case PAGENOTPINNED:
        cerr << "page not pinned";
        break;
    case BADBUFFER:
        cerr << "buffer pool corrupted";
        break;
    case PAGEPINNED:
        cerr << "page still pinned";
        break;

        // Page class errors

    case NOSPACE:
        cerr << "no space on page for record";
        break;
    case NORECORDS:
        cerr << "page is empty - no records";
        break;
    case ENDOFPAGE:
        cerr << "last record on page";
        break;

        // B+ Tree errors
    case BADINDEXPARM:
        cerr << "bad index parameter";
        break;
    case TUPLENOTFOUND:
        cerr << "Tuple was not found!";
        break;
    case NULLROOT:
        cerr << "The root is null";
        break;
    case NOTFOUND:
        cerr << "The tuple is not found within tree";
        break;
    case NOSIBLINGS:
        cerr << "No siblings found";
        break;
    case NULLPTR:
        cerr << "Bad pointer";
        break;

        // Index errors
    case RECNOTFOUND:
        cerr << "no such record";
        break;
    case BUCKETFULL:
        cerr << "bucket full";
        break;
    case DIROVERFLOW:
        cerr << "directory is full";
        break;
    case NONUNIQUEENTRY:
        cerr << "nonunique entry";
        break;
    case NOMORERECS:
        cerr << "no more records";
        break;
    case INDEXNOTFOUND:
        cerr << "The index doesn't exist";
        break;

        // Catalog errors

    case BADCATPARM:
        cerr << "bad catalog parameter";
        break;
    case RELNOTFOUND:
        cerr << "relation not in catalog";
        break;
    case ATTRNOTFOUND:
        cerr << "attribute not in catalog";
        break;
    case NAMETOOLONG:
        cerr << "name too long";
        break;
    case ATTRTOOLONG:
        cerr << "attributes too long";
        break;
    case DUPLATTR:
        cerr << "duplicate attribute names";
        break;
    case RELEXISTS:
        cerr << "relation exists already";
        break;
    case NOINDEX:
        cerr << "no index exists";
        break;
    case ATTRTYPEMISMATCH:
        cerr << "attribute type mismatch";
        break;
    case TMP_RES_EXISTS:
        cerr << "temp result already exists";
        break;
    case INDEXEXISTS:
        cerr << "index exists already";
        break;

    default:
        cerr << "undefined error status: " << status;
    }
    cerr << endl;
}
