#ifndef PLAYDATAMGR_H_
#define PLAYDATAMGR_H_



#include <map>
using namespace std;

typedef map<UINT, DT_PLAYER_DATA*>  CMapDT_PLAYER_DATA;
typedef CMapDT_PLAYER_DATA::iterator CMapDT_PLAYER_DATA_Itr;

class CPlayDataMgr
{
protected:
    CPlayDataMgr();

public:
    ~CPlayDataMgr();
    static CPlayDataMgr * Instance();
    BOOL InitDB();
    BOOL SaveDB(UINT);
    CMapDT_PLAYER_DATA& GetMapDT_PLAYER_DATA()
    {
        return mapDT_PLAYER_DATA;
    }
    void GetVectorID(vector<UINT> & _vecPlay, emOptType _emOptType);
	void GetVectorID(vector<UINT> & _vecPlay, CString _strFound, emOptType _emOptType);
    DT_PLAYER_DATA * GetDT_PLAYER_DATA(UINT uiID);

private:
    static CPlayDataMgr * instance;
private:
    map<UINT, DT_PLAYER_DATA *> mapDT_PLAYER_DATA;
};

#endif