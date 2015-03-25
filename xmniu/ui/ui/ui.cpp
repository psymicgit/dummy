// ui.cpp: 主项目文件。

#include "stdafx.h"
#include "RobotForm.h"

using namespace ui;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// 在创建任何控件之前启用 Windows XP 可视化效果
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// 创建主窗口并运行它
    RobotForm^ robotForm = gcnew RobotForm();
	Application::Run(robotForm);
	return 0;
}
