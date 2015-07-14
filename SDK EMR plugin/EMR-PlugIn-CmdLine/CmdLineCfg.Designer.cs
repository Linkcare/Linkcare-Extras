namespace EMR.PlugIn.CmdLine
{
  partial class CmdLineCfg
  {
    /// <summary> 
    /// Required designer variable.
    /// </summary>
    private System.ComponentModel.IContainer components = null;

    /// <summary> 
    /// Clean up any resources being used.
    /// </summary>
    /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
    protected override void Dispose(bool disposing)
    {
      if (disposing && (components != null))
      {
        components.Dispose();
      }
      base.Dispose(disposing);
    }

    #region Component Designer generated code

    /// <summary> 
    /// Required method for Designer support - do not modify 
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.components = new System.ComponentModel.Container();
      this.errorProviderValidation = new System.Windows.Forms.ErrorProvider(this.components);
      this.label14 = new System.Windows.Forms.Label();
      this.label15 = new System.Windows.Forms.Label();
      this.label13 = new System.Windows.Forms.Label();
      this.comboBoxFileNamePart3 = new System.Windows.Forms.ComboBox();
      this.comboBoxFileNamePart2 = new System.Windows.Forms.ComboBox();
      this.comboBoxFileNamePart1 = new System.Windows.Forms.ComboBox();
      this.comboBoxFormat = new System.Windows.Forms.ComboBox();
      this.groupBoxCfg = new System.Windows.Forms.GroupBox();
      this.textBoxEchangeFolder = new System.Windows.Forms.TextBox();
      this.label12 = new System.Windows.Forms.Label();
      this.checkBoxCloseAfterTest = new System.Windows.Forms.CheckBox();
      this.checkBoxIncludeTrialValues = new System.Windows.Forms.CheckBox();
      this.checkBoxIncludeCurveData = new System.Windows.Forms.CheckBox();
      this.checkBoxAttachPDF = new System.Windows.Forms.CheckBox();
      this.buttonLogin = new System.Windows.Forms.Button();
      this.textBox1 = new System.Windows.Forms.TextBox();
      this.label1 = new System.Windows.Forms.Label();
      ((System.ComponentModel.ISupportInitialize)(this.errorProviderValidation)).BeginInit();
      this.groupBoxCfg.SuspendLayout();
      this.SuspendLayout();
      // 
      // errorProviderValidation
      // 
      this.errorProviderValidation.ContainerControl = this;
      // 
      // label14
      // 
      this.label14.AutoSize = true;
      this.label14.Location = new System.Drawing.Point(397, 196);
      this.label14.Name = "label14";
      this.label14.Size = new System.Drawing.Size(10, 13);
      this.label14.TabIndex = 5;
      this.label14.Text = ".";
      // 
      // label15
      // 
      this.label15.AutoSize = true;
      this.label15.Location = new System.Drawing.Point(410, 169);
      this.label15.Name = "label15";
      this.label15.Size = new System.Drawing.Size(93, 13);
      this.label15.TabIndex = 7;
      this.label15.Text = "Attachment format";
      // 
      // label13
      // 
      this.label13.AutoSize = true;
      this.label13.Location = new System.Drawing.Point(15, 169);
      this.label13.Name = "label13";
      this.label13.Size = new System.Drawing.Size(106, 13);
      this.label13.TabIndex = 6;
      this.label13.Text = "Attachment file name";
      // 
      // comboBoxFileNamePart3
      // 
      this.comboBoxFileNamePart3.FormattingEnabled = true;
      this.comboBoxFileNamePart3.Items.AddRange(new object[] {
            "%PatientID%",
            "%Firstname%",
            "%Lastname%",
            "%OrderID%"});
      this.comboBoxFileNamePart3.Location = new System.Drawing.Point(270, 188);
      this.comboBoxFileNamePart3.Name = "comboBoxFileNamePart3";
      this.comboBoxFileNamePart3.Size = new System.Drawing.Size(121, 21);
      this.comboBoxFileNamePart3.TabIndex = 11;
      // 
      // comboBoxFileNamePart2
      // 
      this.comboBoxFileNamePart2.FormattingEnabled = true;
      this.comboBoxFileNamePart2.Items.AddRange(new object[] {
            "%PatientID%",
            "%Firstname%",
            "%Lastname%",
            "%OrderID%"});
      this.comboBoxFileNamePart2.Location = new System.Drawing.Point(143, 188);
      this.comboBoxFileNamePart2.Name = "comboBoxFileNamePart2";
      this.comboBoxFileNamePart2.Size = new System.Drawing.Size(121, 21);
      this.comboBoxFileNamePart2.TabIndex = 9;
      // 
      // comboBoxFileNamePart1
      // 
      this.comboBoxFileNamePart1.FormattingEnabled = true;
      this.comboBoxFileNamePart1.Items.AddRange(new object[] {
            "%PatientID%",
            "%Firstname%",
            "%Lastname%",
            "%OrderID%"});
      this.comboBoxFileNamePart1.Location = new System.Drawing.Point(16, 188);
      this.comboBoxFileNamePart1.Name = "comboBoxFileNamePart1";
      this.comboBoxFileNamePart1.Size = new System.Drawing.Size(121, 21);
      this.comboBoxFileNamePart1.TabIndex = 12;
      this.comboBoxFileNamePart1.Text = "%PatientID%";
      // 
      // comboBoxFormat
      // 
      this.comboBoxFormat.FormattingEnabled = true;
      this.comboBoxFormat.Items.AddRange(new object[] {
            "PDF",
            "CSV",
            "TXT",
            "RTF",
            "XLS",
            "PNG",
            "GIF",
            "TIF",
            "JPG",
            "MHT"});
      this.comboBoxFormat.Location = new System.Drawing.Point(413, 188);
      this.comboBoxFormat.Name = "comboBoxFormat";
      this.comboBoxFormat.Size = new System.Drawing.Size(121, 21);
      this.comboBoxFormat.TabIndex = 10;
      this.comboBoxFormat.Text = "PDF";
      // 
      // groupBoxCfg
      // 
      this.groupBoxCfg.Controls.Add(this.label14);
      this.groupBoxCfg.Controls.Add(this.textBoxEchangeFolder);
      this.groupBoxCfg.Controls.Add(this.label15);
      this.groupBoxCfg.Controls.Add(this.label12);
      this.groupBoxCfg.Controls.Add(this.label13);
      this.groupBoxCfg.Controls.Add(this.checkBoxCloseAfterTest);
      this.groupBoxCfg.Controls.Add(this.comboBoxFileNamePart3);
      this.groupBoxCfg.Controls.Add(this.checkBoxIncludeTrialValues);
      this.groupBoxCfg.Controls.Add(this.comboBoxFileNamePart2);
      this.groupBoxCfg.Controls.Add(this.checkBoxIncludeCurveData);
      this.groupBoxCfg.Controls.Add(this.comboBoxFileNamePart1);
      this.groupBoxCfg.Controls.Add(this.checkBoxAttachPDF);
      this.groupBoxCfg.Controls.Add(this.comboBoxFormat);
      this.groupBoxCfg.Enabled = false;
      this.groupBoxCfg.Location = new System.Drawing.Point(13, 56);
      this.groupBoxCfg.Name = "groupBoxCfg";
      this.groupBoxCfg.Size = new System.Drawing.Size(544, 218);
      this.groupBoxCfg.TabIndex = 8;
      this.groupBoxCfg.TabStop = false;
      this.groupBoxCfg.Text = "Configuration";
      // 
      // textBoxEchangeFolder
      // 
      this.textBoxEchangeFolder.Location = new System.Drawing.Point(16, 113);
      this.textBoxEchangeFolder.Name = "textBoxEchangeFolder";
      this.textBoxEchangeFolder.Size = new System.Drawing.Size(520, 20);
      this.textBoxEchangeFolder.TabIndex = 1;
      // 
      // label12
      // 
      this.label12.AutoSize = true;
      this.label12.Location = new System.Drawing.Point(15, 97);
      this.label12.Name = "label12";
      this.label12.Size = new System.Drawing.Size(102, 13);
      this.label12.TabIndex = 1;
      this.label12.Text = "File exchange folder";
      // 
      // checkBoxCloseAfterTest
      // 
      this.checkBoxCloseAfterTest.AutoSize = true;
      this.checkBoxCloseAfterTest.Checked = true;
      this.checkBoxCloseAfterTest.CheckState = System.Windows.Forms.CheckState.Checked;
      this.checkBoxCloseAfterTest.Location = new System.Drawing.Point(16, 19);
      this.checkBoxCloseAfterTest.Name = "checkBoxCloseAfterTest";
      this.checkBoxCloseAfterTest.Size = new System.Drawing.Size(101, 17);
      this.checkBoxCloseAfterTest.TabIndex = 0;
      this.checkBoxCloseAfterTest.Text = "Close After Test";
      this.checkBoxCloseAfterTest.UseVisualStyleBackColor = true;
      // 
      // checkBoxIncludeTrialValues
      // 
      this.checkBoxIncludeTrialValues.AutoSize = true;
      this.checkBoxIncludeTrialValues.Location = new System.Drawing.Point(16, 65);
      this.checkBoxIncludeTrialValues.Name = "checkBoxIncludeTrialValues";
      this.checkBoxIncludeTrialValues.Size = new System.Drawing.Size(118, 17);
      this.checkBoxIncludeTrialValues.TabIndex = 0;
      this.checkBoxIncludeTrialValues.Text = "Include Trial values";
      this.checkBoxIncludeTrialValues.UseVisualStyleBackColor = true;
      // 
      // checkBoxIncludeCurveData
      // 
      this.checkBoxIncludeCurveData.AutoSize = true;
      this.checkBoxIncludeCurveData.Location = new System.Drawing.Point(16, 42);
      this.checkBoxIncludeCurveData.Name = "checkBoxIncludeCurveData";
      this.checkBoxIncludeCurveData.Size = new System.Drawing.Size(118, 17);
      this.checkBoxIncludeCurveData.TabIndex = 0;
      this.checkBoxIncludeCurveData.Text = "Include Curve Data";
      this.checkBoxIncludeCurveData.UseVisualStyleBackColor = true;
      // 
      // checkBoxAttachPDF
      // 
      this.checkBoxAttachPDF.AutoSize = true;
      this.checkBoxAttachPDF.Checked = true;
      this.checkBoxAttachPDF.CheckState = System.Windows.Forms.CheckState.Checked;
      this.checkBoxAttachPDF.Location = new System.Drawing.Point(16, 149);
      this.checkBoxAttachPDF.Name = "checkBoxAttachPDF";
      this.checkBoxAttachPDF.Size = new System.Drawing.Size(81, 17);
      this.checkBoxAttachPDF.TabIndex = 4;
      this.checkBoxAttachPDF.Text = "Attach PDF";
      this.checkBoxAttachPDF.UseVisualStyleBackColor = true;
      // 
      // buttonLogin
      // 
      this.buttonLogin.Location = new System.Drawing.Point(193, 27);
      this.buttonLogin.Name = "buttonLogin";
      this.buttonLogin.Size = new System.Drawing.Size(75, 23);
      this.buttonLogin.TabIndex = 9;
      this.buttonLogin.Text = "Login";
      this.buttonLogin.UseVisualStyleBackColor = true;
      this.buttonLogin.Click += new System.EventHandler(this.buttonLogin_Click);
      // 
      // textBox1
      // 
      this.textBox1.Location = new System.Drawing.Point(13, 29);
      this.textBox1.Name = "textBox1";
      this.textBox1.PasswordChar = '*';
      this.textBox1.Size = new System.Drawing.Size(174, 20);
      this.textBox1.TabIndex = 1;
      this.textBox1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.textBox1_KeyDown);
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(10, 13);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(53, 13);
      this.label1.TabIndex = 6;
      this.label1.Text = "Password";
      // 
      // CmdLineCfg
      // 
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
      this.AutoScroll = true;
      this.AutoSize = true;
      this.Controls.Add(this.buttonLogin);
      this.Controls.Add(this.textBox1);
      this.Controls.Add(this.groupBoxCfg);
      this.Controls.Add(this.label1);
      this.Name = "CmdLineCfg";
      this.Size = new System.Drawing.Size(572, 289);
      ((System.ComponentModel.ISupportInitialize)(this.errorProviderValidation)).EndInit();
      this.groupBoxCfg.ResumeLayout(false);
      this.groupBoxCfg.PerformLayout();
      this.ResumeLayout(false);
      this.PerformLayout();

    }

    #endregion

    private System.Windows.Forms.ErrorProvider errorProviderValidation;
    private System.Windows.Forms.Label label14;
    private System.Windows.Forms.Label label15;
    private System.Windows.Forms.Label label13;
    private System.Windows.Forms.ComboBox comboBoxFileNamePart3;
    private System.Windows.Forms.ComboBox comboBoxFileNamePart2;
    private System.Windows.Forms.ComboBox comboBoxFileNamePart1;
    private System.Windows.Forms.ComboBox comboBoxFormat;
    private System.Windows.Forms.GroupBox groupBoxCfg;
    private System.Windows.Forms.TextBox textBoxEchangeFolder;
    private System.Windows.Forms.Label label12;
    private System.Windows.Forms.CheckBox checkBoxCloseAfterTest;
    private System.Windows.Forms.CheckBox checkBoxIncludeTrialValues;
    private System.Windows.Forms.CheckBox checkBoxIncludeCurveData;
    private System.Windows.Forms.CheckBox checkBoxAttachPDF;
    private System.Windows.Forms.Button buttonLogin;
    private System.Windows.Forms.TextBox textBox1;
    private System.Windows.Forms.Label label1;
  }
}
