#pragma once

#include <string>

#include "robot/mission.h"
#include "robot/robot.h"
#include "robot/util.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;

using namespace logutil;

//建立委托
delegate void log_delegate(String^ str);

delegate void progress_delegate(int progress);

/*
void log_info(char buf[])
{
    String ^str = gcnew String(buf);
    (*g_listView)->Items->Add(gcnew ListViewItem(str));
    delete str;
}
}*/

/*
        String ^str = gcnew String(msg_buf); \
        (*g_listView)->Items->Add(gcnew ListViewItem(str)); \
        delete str; \
*/

#define LOG_TRACE(xxx, ...) \
    if(log_or_not) \
    {\
        char msg_buf[2048]; \
        sprintf_s(msg_buf, sizeof(msg_buf), xxx, __VA_ARGS__); \
        delegate_log(msg_buf); \
    }

#define LOG_ERR(xxx, ...) LOG_TRACE(xxx, __VA_ARGS__)

// delegate void log_info(int i);
// myDelegate mydelegate = null;

namespace ui {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Runtime::InteropServices;

    /// <summary>
    /// RobotForm 摘要
    ///
    /// 警告: 如果更改此类的名称，则需要更改
    ///          与此类所依赖的所有 .resx 文件关联的托管资源编译器工具的
    ///          “资源文件名”属性。否则，
    ///          设计器将不能与此窗体的关联
    ///          本地化资源正确交互。
    /// </summary>
    public ref class RobotForm : public System::Windows::Forms::Form
    {
    public:
        RobotForm(void)
        {
            InitializeComponent();
            //
            //TODO: 在此处添加构造函数代码
            //
            
            *g_robotForm = this;
        }
    private: System::Windows::Forms::TextBox^  textBox4;
    public: 
    private: System::Windows::Forms::Label^  label1;
    private: System::Windows::Forms::Label^  label2;
    public: System::Windows::Forms::ProgressBar^  progressBar1;

    public:
        static  gcroot<RobotForm^> *g_robotForm = new gcroot<RobotForm^>;


    protected:
        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        ~RobotForm()
        {
            if (components)
            {
                delete components;
            }
        }
    public: System::Windows::Forms::Button^  poweron;
    protected: 

    protected: 

    protected: 

    public: System::Windows::Forms::ListView^  listView1;
    public: System::Windows::Forms::ToolTip^  toolTip1;

    public: System::Windows::Forms::ColumnHeader^  columnHeader1;
    public: System::Windows::Forms::ColumnHeader^  columnHeader2;
    public: System::Windows::Forms::ImageList^  imageList1;
    public: System::Windows::Forms::Button^  button1;
    public: System::Windows::Forms::HScrollBar^  hScrollBar1;
    public: System::Windows::Forms::TextBox^  textBox1;
    public: System::Windows::Forms::CheckBox^  checkBox1;
    public: System::Windows::Forms::TextBox^  textBox2;
    public: System::Windows::Forms::TextBox^  textBox3;




    private: System::ComponentModel::IContainer^  components;


    private:
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>


#pragma region Windows Form Designer generated code
        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        void InitializeComponent(void)
        {
            this->components = (gcnew System::ComponentModel::Container());
            System::Windows::Forms::ListViewGroup^  listViewGroup3 = (gcnew System::Windows::Forms::ListViewGroup(L"ListViewGroup", System::Windows::Forms::HorizontalAlignment::Left));
            System::Windows::Forms::ListViewGroup^  listViewGroup4 = (gcnew System::Windows::Forms::ListViewGroup(L"ListViewGroup", System::Windows::Forms::HorizontalAlignment::Left));
            System::Windows::Forms::ListViewItem^  listViewItem3 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(3) {L"dfgsdfg", 
                L"1111", L"2222"}, 0));
            System::Windows::Forms::ListViewItem^  listViewItem4 = (gcnew System::Windows::Forms::ListViewItem(gcnew cli::array< System::String^  >(3) {L"sdfdf", 
                L"1", L"2"}, -1));
            System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(RobotForm::typeid));
            this->poweron = (gcnew System::Windows::Forms::Button());
            this->listView1 = (gcnew System::Windows::Forms::ListView());
            this->columnHeader1 = (gcnew System::Windows::Forms::ColumnHeader());
            this->columnHeader2 = (gcnew System::Windows::Forms::ColumnHeader());
            this->imageList1 = (gcnew System::Windows::Forms::ImageList(this->components));
            this->toolTip1 = (gcnew System::Windows::Forms::ToolTip(this->components));
            this->button1 = (gcnew System::Windows::Forms::Button());
            this->hScrollBar1 = (gcnew System::Windows::Forms::HScrollBar());
            this->textBox1 = (gcnew System::Windows::Forms::TextBox());
            this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
            this->textBox2 = (gcnew System::Windows::Forms::TextBox());
            this->textBox3 = (gcnew System::Windows::Forms::TextBox());
            this->textBox4 = (gcnew System::Windows::Forms::TextBox());
            this->label1 = (gcnew System::Windows::Forms::Label());
            this->label2 = (gcnew System::Windows::Forms::Label());
            this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
            this->SuspendLayout();
            // 
            // poweron
            // 
            this->poweron->Location = System::Drawing::Point(40, 121);
            this->poweron->Name = L"poweron";
            this->poweron->Size = System::Drawing::Size(132, 55);
            this->poweron->TabIndex = 0;
            this->poweron->Text = L"启动机器人";
            this->poweron->UseVisualStyleBackColor = true;
            this->poweron->Click += gcnew System::EventHandler(this, &RobotForm::poweron_Click);
            // 
            // listView1
            // 
            this->listView1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->listView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::ColumnHeader^  >(2) {this->columnHeader1, this->columnHeader2});
            listViewGroup3->Header = L"ListViewGroup";
            listViewGroup3->Name = L"listViewGroup1";
            listViewGroup3->Tag = L"1111";
            listViewGroup4->Header = L"ListViewGroup";
            listViewGroup4->Name = L"listViewGroup2";
            listViewGroup4->Tag = L"2222";
            this->listView1->Groups->AddRange(gcnew cli::array< System::Windows::Forms::ListViewGroup^  >(2) {listViewGroup3, listViewGroup4});
            listViewItem3->IndentCount = 1;
            this->listView1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ListViewItem^  >(2) {listViewItem3, listViewItem4});
            this->listView1->LargeImageList = this->imageList1;
            this->listView1->Location = System::Drawing::Point(656, 96);
            this->listView1->Name = L"listView1";
            this->listView1->Size = System::Drawing::Size(252, 366);
            this->listView1->TabIndex = 2;
            this->listView1->UseCompatibleStateImageBehavior = false;
            this->listView1->View = System::Windows::Forms::View::Details;
            this->listView1->SelectedIndexChanged += gcnew System::EventHandler(this, &RobotForm::listView1_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this->columnHeader1->Text = L"结果";
            this->columnHeader1->Width = 153;
            // 
            // columnHeader2
            // 
            this->columnHeader2->Text = L"数量";
            // 
            // imageList1
            // 
            this->imageList1->ImageStream = (cli::safe_cast<System::Windows::Forms::ImageListStreamer^  >(resources->GetObject(L"imageList1.ImageStream")));
            this->imageList1->TransparentColor = System::Drawing::Color::Transparent;
            this->imageList1->Images->SetKeyName(0, L"shortcut.jpg");
            // 
            // button1
            // 
            this->button1->Anchor = System::Windows::Forms::AnchorStyles::None;
            this->button1->CausesValidation = false;
            this->button1->Enabled = false;
            this->button1->Location = System::Drawing::Point(588, 194);
            this->button1->Name = L"button1";
            this->button1->Size = System::Drawing::Size(58, 23);
            this->button1->TabIndex = 3;
            this->button1->Text = L"-->";
            this->button1->UseVisualStyleBackColor = true;
            this->button1->Click += gcnew System::EventHandler(this, &RobotForm::button1_Click);
            // 
            // hScrollBar1
            // 
            this->hScrollBar1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->hScrollBar1->LargeChange = 1;
            this->hScrollBar1->Location = System::Drawing::Point(40, 47);
            this->hScrollBar1->Maximum = 50000;
            this->hScrollBar1->Minimum = 1;
            this->hScrollBar1->Name = L"hScrollBar1";
            this->hScrollBar1->Size = System::Drawing::Size(856, 33);
            this->hScrollBar1->TabIndex = 4;
            this->hScrollBar1->Value = 1;
            this->hScrollBar1->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &RobotForm::hScrollBar1_Scroll);
            // 
            // textBox1
            // 
            this->textBox1->CausesValidation = false;
            this->textBox1->Enabled = false;
            this->textBox1->Font = (gcnew System::Drawing::Font(L"宋体", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            this->textBox1->Location = System::Drawing::Point(386, 12);
            this->textBox1->Name = L"textBox1";
            this->textBox1->Size = System::Drawing::Size(100, 26);
            this->textBox1->TabIndex = 5;
            this->textBox1->Text = L"1";
            this->textBox1->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->textBox1->TextChanged += gcnew System::EventHandler(this, &RobotForm::textBox1_TextChanged);
            // 
            // checkBox1
            // 
            this->checkBox1->AutoSize = true;
            this->checkBox1->Checked = true;
            this->checkBox1->CheckState = System::Windows::Forms::CheckState::Checked;
            this->checkBox1->Font = (gcnew System::Drawing::Font(L"宋体", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            this->checkBox1->Location = System::Drawing::Point(41, 194);
            this->checkBox1->Name = L"checkBox1";
            this->checkBox1->Size = System::Drawing::Size(91, 20);
            this->checkBox1->TabIndex = 6;
            this->checkBox1->Text = L"打印日志";
            this->checkBox1->UseVisualStyleBackColor = true;
            this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &RobotForm::checkBox1_CheckedChanged);
            // 
            // textBox2
            // 
            this->textBox2->Font = (gcnew System::Drawing::Font(L"微软雅黑", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
                static_cast<System::Byte>(134)));
            this->textBox2->Location = System::Drawing::Point(35, 267);
            this->textBox2->Name = L"textBox2";
            this->textBox2->Size = System::Drawing::Size(132, 29);
            this->textBox2->TabIndex = 7;
            this->textBox2->Text = L"127.0.0.1";
            this->textBox2->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            this->textBox2->TextChanged += gcnew System::EventHandler(this, &RobotForm::textBox2_TextChanged);
            // 
            // textBox3
            // 
            this->textBox3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
                | System::Windows::Forms::AnchorStyles::Left) 
                | System::Windows::Forms::AnchorStyles::Right));
            this->textBox3->Location = System::Drawing::Point(188, 96);
            this->textBox3->Multiline = true;
            this->textBox3->Name = L"textBox3";
            this->textBox3->ScrollBars = System::Windows::Forms::ScrollBars::Both;
            this->textBox3->Size = System::Drawing::Size(389, 365);
            this->textBox3->TabIndex = 9;
            this->textBox3->TextChanged += gcnew System::EventHandler(this, &RobotForm::textBox3_TextChanged);
            // 
            // textBox4
            // 
            this->textBox4->Location = System::Drawing::Point(46, 336);
            this->textBox4->Name = L"textBox4";
            this->textBox4->Size = System::Drawing::Size(100, 21);
            this->textBox4->TabIndex = 10;
            this->textBox4->Text = L"8087";
            this->textBox4->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
            // 
            // label1
            // 
            this->label1->AutoSize = true;
            this->label1->Location = System::Drawing::Point(22, 318);
            this->label1->Name = L"label1";
            this->label1->Size = System::Drawing::Size(41, 12);
            this->label1->TabIndex = 11;
            this->label1->Text = L"端口：";
            this->label1->Click += gcnew System::EventHandler(this, &RobotForm::label1_Click_1);
            // 
            // label2
            // 
            this->label2->AutoSize = true;
            this->label2->Location = System::Drawing::Point(21, 243);
            this->label2->Name = L"label2";
            this->label2->Size = System::Drawing::Size(41, 12);
            this->label2->TabIndex = 12;
            this->label2->Text = L"地址：";
            // 
            // progressBar1
            // 
            this->progressBar1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
            this->progressBar1->Location = System::Drawing::Point(521, 12);
            this->progressBar1->Name = L"progressBar1";
            this->progressBar1->Size = System::Drawing::Size(287, 23);
            this->progressBar1->TabIndex = 13;
            this->progressBar1->Click += gcnew System::EventHandler(this, &RobotForm::progressBar1_Click);
            // 
            // RobotForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(920, 473);
            this->Controls->Add(this->progressBar1);
            this->Controls->Add(this->label2);
            this->Controls->Add(this->label1);
            this->Controls->Add(this->textBox4);
            this->Controls->Add(this->textBox3);
            this->Controls->Add(this->textBox2);
            this->Controls->Add(this->checkBox1);
            this->Controls->Add(this->textBox1);
            this->Controls->Add(this->hScrollBar1);
            this->Controls->Add(this->button1);
            this->Controls->Add(this->listView1);
            this->Controls->Add(this->poweron);
            this->Name = L"RobotForm";
            this->Text = L"机器人";
            this->Load += gcnew System::EventHandler(this, &RobotForm::RobotForm_Load);
            this->ResumeLayout(false);
            this->PerformLayout();

        }
#pragma endregion
        //判断一下是不是该用Invoke滴~，不是就直接返回~

    private: System::Void listBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             }
    private: System::Void listView1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
             }
    private: System::Void RobotForm_Load(System::Object^  sender, System::EventArgs^  e) {
                 System::Windows::Forms::Control::CheckForIllegalCrossThreadCalls = false;
             }
    private: System::Void poweron_Click(System::Object^  sender, System::EventArgs^  e) {
                 // this->listView1->View = View::Details;

                 // this->listView1->Columns->Add("机器人");
                 //this->listView1->Items->Add(gcnew ListViewItem("aaaa"));// = View.Details; //listView1.View =View.Details;
                 int i = hScrollBar1->Value;
                 std::string aaa = "aaa";

                 *g_textBox = this->textBox3;

                 this->progressBar1->Value = 0;
                 this->progressBar1->Refresh();
                 // this->listBox1->Items.Add(new Listite)

                 // this->listView1->Items->Add(gcnew ListViewItem("aaaa"));// = View.Details; //listView1.View =

/*
                 ListViewItem^ item = this->listView1->Items[0];// gcnew ListViewItem("aaa");

                 item->SubItems[0]->Text = "成功机器人数量";
                 item->SubItems[1]->Text = "" + i++;

                 ListViewItem^ item2 = this->listView1->Items[1];// gcnew ListViewItem("aaa");

                 item2->SubItems[0]->Text = "失败机器人数量";
                 item2->SubItems[1]->Text = "" + i++;
*/
                 this->poweron->Enabled = false;

                 int n_robot = this->hScrollBar1->Value;
                 bool log_all = this->checkBox1->Checked;

                 svr_t *svrs = new svr_t[1];

                 String^ str = textBox2->Text; 
                 char *svr_addr = (char*)(void*)Marshal::StringToHGlobalAnsi(str);

                 unsigned long cast_addr = name_resolve(svr_addr);
                 if(INADDR_NONE  == cast_addr)
                 {
                     LOG_ERR("parse %dth argument fail: <%s> is not a valid address\r\n", i, svr_addr);
                     return;
                 }

                 int port = Convert::ToInt32(textBox4->Text);
                 if(port <= 0 )
                 {
                     String^ port_text = textBox4->Text; 
                     char *port_str = (char*)(void*)Marshal::StringToHGlobalAnsi(port_text);

                     LOG_ERR("parse port fail: <%s> is not a valid port\r\n", port_str);
                     return;
                 }

                 svr_t &svr = svrs[0];
                 svr.addr = svr_addr;
                 svr.port = port;
                 svr.cast_addr = cast_addr;

                 mission_t *mission = new mission_t(n_robot, log_all, svrs, 1);
                 mission->init();
                 mission->start();
             }

    private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e) {
             }

#define max(a,b)    (((a) > (b)) ? (a) : (b))
             int calc_diff(int xxx)
             {
                 int diff = 10;
                 if(xxx < 10)
                 {
                     diff = 1;
                 }
                 else if(xxx < 100)
                 {
                     diff = 10;
                 }
                 else if(xxx < 1000)
                 {
                     diff = 100;
                 }
                 else
                 {
                     diff = 1000;
                 }

                 return diff;
             }

    private: System::Void hScrollBar1_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e)            {
                 // this->textBox1->Text = e->NewValue.ToString();//this->hScrollBar1->Value.ToString()

                 int diff = calc_diff(e->OldValue);

                 switch(e->Type)
                 {
                 case ScrollEventType::SmallDecrement:
                 case ScrollEventType::LargeDecrement:
                     e->NewValue = max( hScrollBar1->Minimum, e->OldValue - diff);
                     // this->hScrollBar1->Value = e->OldValue - diff;

                     break;

                 case ScrollEventType::SmallIncrement:
                 case ScrollEventType::LargeIncrement:
                     e->NewValue = min(hScrollBar1->Maximum, e->OldValue + diff);
                     //this->hScrollBar1->Value = e->OldValue + diff;
                     break;

                 case ScrollEventType::ThumbPosition:
                 case ScrollEventType::ThumbTrack:
                 case ScrollEventType::EndScroll:
                     e->NewValue = e->NewValue - e->NewValue % calc_diff(e->NewValue);
                     e->NewValue = max(e->NewValue, hScrollBar1->Minimum);

                     break;
                 }
                 /*
                 if(e->NewValue < this->hScrollBar1->Maximum / 10)
                 {
                 this->hScrollBar1->Value = e->NewValue - e->NewValue % 10;
                 // e->NewValue = 30000;

                 }
                 else
                 {
                 this->hScrollBar1->Value = e->NewValue - e->NewValue % 100;
                 }
                 */

                 this->hScrollBar1->Refresh();

                 this->textBox1->Text = hScrollBar1->Value.ToString();
            }
    private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
            }
    private: System::Void textBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
            }
    private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
            }
    private: System::Void textBox3_TextChanged(System::Object^  sender, System::EventArgs^  e) {
             }
private: System::Void label1_Click_1(System::Object^  sender, System::EventArgs^  e) {
         }
private: System::Void progressBar1_Click(System::Object^  sender, System::EventArgs^  e) {
         }
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
         }
};
}

