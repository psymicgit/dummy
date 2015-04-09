// PacketRobotDlg.h : header file
//
#ifndef PACKETROBOTDLG_H_
#define PACKETROBOTDLG_H_
#pragma once

#include "afxcmn.h"
#include "protocolmgr.h"
#include "clipktparser.h"
#include "Resource.h"
#include <list>

using namespace SGDP;

// 最大日志长度
#define LOG_MAX_LENGTH 102400
// 输入框个数
#define INPUT_MAX_COUNT 8
// 引导完成发送时延
#define TIMER_ID_LOADFINISH 1
// 心跳包时延
#define TIMER_ID_HEARTBEAT 2
// 频繁移动
#define TIMER_ID_MOVEVAST 3

// 日志类型
typedef enum eLogType
{
	LOG_DATA,
	LOG_DESCRIBE,
	LOG_ERROR,
	LOG_NOPRINT
}LogType;

// CPacketRobotDlg dialog
class CPacketRobotDlg : public CDialog
{
// Construction
public:
	CPacketRobotDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PACKETROBOT_DIALOG };

protected:
	virtual VOID DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	ISDNet* m_pISDClient;
    ISDConnector* m_poConnector;

	// Generated message map functions
	virtual SDBOOL OnInitDialog();
	afx_msg VOID OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	// ---------------- 回  调 ----------------

	// 连接服务器
	UINT32 ConnectServer(const CHAR* _IP, const UINT32 _Port);
	// 断开连接
	VOID CutOffServer();
	// 连接成功
	virtual VOID SDAPI OnConnect(INT32 nErrorCode);
	// 断开连接
	virtual VOID SDAPI OnDisConnect(INT32 nErrorCode);
	// 接收数据
	virtual VOID SDAPI OnRecvMsg(UINT16 wMsgID, const CHAR* pBuf, INT32 nLen);

	// ---------------- 测试协议 ----------------
	// 发起心跳
	VOID SendHeartBeat(UINT8 _ExtInfo = 0);
	// 校对时间
	VOID SendCheckTime();


    // 发送常规消息
    VOID Send_COMMON(INT32 nMsgID, VOID* pReq, const CHAR* szDescription);

	// ---------------- 辅助方法 ----------------

	// 打印日志
	VOID PrintText(const CHAR* cpText, const LogType _LogType = LOG_DATA);
	// 打印日志
	VOID WriteText(const TCHAR* cpText, const LogType _LogType = LOG_DATA);
	// 解析结构体日志
	VOID ParseLog(const CHAR* _Log);
	// 刷新协议表
	VOID FlushList();


	// 配置输入框
	VOID AdjustEdit(INT32 _Index, CHAR* Caption, CHAR* Value);
	// 清空输入框
	VOID ClearInput();
	// 配置输入
	VOID AdjustInput(INT32 _Index);

public:
	// ---------------- UI接口 ----------------

	// 连接服务器
	afx_msg VOID OnBnClickedOk();
	// 断开
	afx_msg void OnBnClickedButtonCut();
	// 发送报文
	afx_msg VOID OnBnClickedButtonSend();
	// 清空消息框
	afx_msg void OnBnClickedButtonClear();
	// 退出
	afx_msg VOID OnBnClickedCancel();
	// 选中协议
	afx_msg VOID OnNMClickListSelect(NMHDR *pNMHDR, LRESULT *pResult);
	// 选中结构体
	afx_msg void OnTvnSelchangedTreeStruct(NMHDR *pNMHDR, LRESULT *pResult);
	// 
	afx_msg VOID OnTimer(UINT_PTR nIDEvent);

private:
	// 列表控件
	CListCtrl m_lcList;
	// Rich Edit
	CRichEditCtrl m_richEdit;
	// 结构体展示
	CTreeCtrl m_Tree;
	// 资源析构
	std::list<CHAR*> m_LsResource;
	// 用户Id缓存
	CHAR m_szUserId[USERNAME_LEN];
	
};

#endif

