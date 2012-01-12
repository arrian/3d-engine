#pragma once

namespace NH2012Server {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class ServerGUI : public System::Windows::Forms::Form
	{
	public:
		ServerGUI(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ServerGUI()
		{
			if (components)
			{
				delete components;
			}
		}
  private: System::Windows::Forms::Panel^  panel1;
  protected: 
  private: System::Windows::Forms::Label^  label1;
  private: System::Windows::Forms::TextBox^  textBox1;
  private: System::Windows::Forms::Panel^  panel2;
  private: System::Windows::Forms::StatusStrip^  statusStrip1;
  private: System::Windows::Forms::MaskedTextBox^  maskedTextBox1;
  private: System::Windows::Forms::Button^  button1;
  private: System::Windows::Forms::Label^  label2;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
      this->panel1 = (gcnew System::Windows::Forms::Panel());
      this->panel2 = (gcnew System::Windows::Forms::Panel());
      this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
      this->textBox1 = (gcnew System::Windows::Forms::TextBox());
      this->label1 = (gcnew System::Windows::Forms::Label());
      this->label2 = (gcnew System::Windows::Forms::Label());
      this->button1 = (gcnew System::Windows::Forms::Button());
      this->maskedTextBox1 = (gcnew System::Windows::Forms::MaskedTextBox());
      this->panel1->SuspendLayout();
      this->SuspendLayout();
      // 
      // panel1
      // 
      this->panel1->Controls->Add(this->maskedTextBox1);
      this->panel1->Controls->Add(this->button1);
      this->panel1->Controls->Add(this->label2);
      this->panel1->Controls->Add(this->label1);
      this->panel1->Controls->Add(this->textBox1);
      this->panel1->Location = System::Drawing::Point(63, 55);
      this->panel1->Name = L"panel1";
      this->panel1->Size = System::Drawing::Size(286, 369);
      this->panel1->TabIndex = 0;
      // 
      // panel2
      // 
      this->panel2->Location = System::Drawing::Point(364, 55);
      this->panel2->Name = L"panel2";
      this->panel2->Size = System::Drawing::Size(370, 369);
      this->panel2->TabIndex = 1;
      // 
      // statusStrip1
      // 
      this->statusStrip1->Location = System::Drawing::Point(0, 503);
      this->statusStrip1->Name = L"statusStrip1";
      this->statusStrip1->Size = System::Drawing::Size(771, 22);
      this->statusStrip1->TabIndex = 2;
      this->statusStrip1->Text = L"statusStrip1";
      // 
      // textBox1
      // 
      this->textBox1->Location = System::Drawing::Point(75, 14);
      this->textBox1->Name = L"textBox1";
      this->textBox1->Size = System::Drawing::Size(208, 22);
      this->textBox1->TabIndex = 0;
      // 
      // label1
      // 
      this->label1->AutoSize = true;
      this->label1->Location = System::Drawing::Point(4, 17);
      this->label1->Name = L"label1";
      this->label1->Size = System::Drawing::Size(45, 17);
      this->label1->TabIndex = 1;
      this->label1->Text = L"Name";
      // 
      // label2
      // 
      this->label2->AutoSize = true;
      this->label2->Location = System::Drawing::Point(4, 45);
      this->label2->Name = L"label2";
      this->label2->Size = System::Drawing::Size(69, 17);
      this->label2->TabIndex = 3;
      this->label2->Text = L"Password";
      // 
      // button1
      // 
      this->button1->Location = System::Drawing::Point(7, 68);
      this->button1->Name = L"button1";
      this->button1->Size = System::Drawing::Size(276, 36);
      this->button1->TabIndex = 4;
      this->button1->Text = L"Start Server";
      this->button1->UseVisualStyleBackColor = true;
      // 
      // maskedTextBox1
      // 
      this->maskedTextBox1->Location = System::Drawing::Point(75, 42);
      this->maskedTextBox1->Name = L"maskedTextBox1";
      this->maskedTextBox1->Size = System::Drawing::Size(208, 22);
      this->maskedTextBox1->TabIndex = 5;
      // 
      // ServerGUI
      // 
      this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
      this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
      this->ClientSize = System::Drawing::Size(771, 525);
      this->Controls->Add(this->statusStrip1);
      this->Controls->Add(this->panel2);
      this->Controls->Add(this->panel1);
      this->Name = L"ServerGUI";
      this->Text = L"NH2012 Server";
      this->panel1->ResumeLayout(false);
      this->panel1->PerformLayout();
      this->ResumeLayout(false);
      this->PerformLayout();

    }
#pragma endregion
	};
}

