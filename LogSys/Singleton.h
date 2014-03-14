
// -*- C++ -*-
//==========================================================
/**
 *     Created_datetime : 4/12/2013 9:00
 *     File_base        : Singleton
 *     File_ext         : h
 *     Author           : GNUnix <Kingbug2010@gmail.com>
 *     Description      :
 *
 *     <Change_list>
 */
//==========================================================
#ifndef _Singleton_hpp_
#define _Singleton_hpp_

template<class T>
class Singleton
{
public:
    static T *getInstancePtr()
    {
        if ( nullptr == instance )
        {
            T *ins = instance;
            if ( nullptr == ins )
            {
                instance = new T;
            }
        }

        return instance;
    }

    static void releaseInstance()
    {
        if ( nullptr == instance )
        {
            delete instance;
            instance = nullptr;
        }
    }

public:
    static T *instance;
} ;


#endif