///<------------------------------------------------------------------------------
//< @file:  %cfg%.h
//< @brief: 本头文件由工具自动生成，请勿在本文件内做改动
//< @Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _%cfg%_h_
#define _%cfg%_h_

#include <string>

#include <map>
#include <vector>
#include <set>

%structs_part%

// %cfg_member%
class %cfg%mgr
{
public:
%mgr_typedefs_part%

public:
	%cfg%mgr(){}
	~%cfg%mgr(){}

	static %cfg%mgr& instance(){
		static %cfg%mgr static_%cfg%mgr;
		return static_%cfg%mgr;
	}

private:
    %cfg%mgr& operator=(const %cfg%mgr&){}
    %cfg%mgr(const %cfg%mgr&){}

public:
	// @impl 获取本配置类名称
	std::string mgr_name(){ return "%cfg%"; }

    // 获取本配置所在路径
    std::string get_path(std::string xml){ return "../../xml/" + xml;}

public:
    // 载入%cfg_member%
	bool load();

    // 清空%cfg_member%
    void clear();

// 载入方法
public:
%mgr_load_funcs_part%

// 清空方法
public:
%mgr_clear_funcs_part%

// 查找方法
public:
%mgr_find_funcs_part%

// 获取成员方法
public:
%mgr_get_funcs_part%

// 成员变量区
public:
%mgr_members_part%
};

#endif // _%cfg%_h_
