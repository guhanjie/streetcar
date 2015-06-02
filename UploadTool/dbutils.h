#ifndef DBUTILS_H
#define DBUTILS_H

class DBUtils
{
    public:
        DBUtils();
        ~DBUtils();
        bool connectDB();
        void closeDB();
};

#endif // DBUTILS_H
