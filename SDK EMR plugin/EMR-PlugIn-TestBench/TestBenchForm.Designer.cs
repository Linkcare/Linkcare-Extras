namespace EMR.PlugIn.TestBench
{
  partial class TestBenchForm
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

    #region Windows Form Designer generated code

    /// <summary>
    /// Required method for Designer support - do not modify
    /// the contents of this method with the code editor.
    /// </summary>
    private void InitializeComponent()
    {
      this.tabPageEmrSetup = new System.Windows.Forms.TabPage();
      this.label16 = new System.Windows.Forms.Label();
      this.checkBoxSupportSearchPatient = new System.Windows.Forms.CheckBox();
      this.checkBoxSupportWorklist = new System.Windows.Forms.CheckBox();
      this.tabPageEmrMessage = new System.Windows.Forms.TabPage();
      this.panel1 = new System.Windows.Forms.Panel();
      this.label17 = new System.Windows.Forms.Label();
      this.textBoxTestTime = new System.Windows.Forms.TextBox();
      this.button1 = new System.Windows.Forms.Button();
      this.buttonPrintPreview = new System.Windows.Forms.Button();
      this.maskedTextBoxHeight = new System.Windows.Forms.MaskedTextBox();
      this.numericUpDownWeight = new System.Windows.Forms.NumericUpDown();
      this.buttonGetTest = new System.Windows.Forms.Button();
      this.buttonShowTest = new System.Windows.Forms.Button();
      this.buttonPerformTestType = new System.Windows.Forms.Button();
      this.buttonPerformTest = new System.Windows.Forms.Button();
      this.richTextBoxReceivedMessages = new System.Windows.Forms.RichTextBox();
      this.dateTimePicker1 = new System.Windows.Forms.DateTimePicker();
      this.comboBoxTestType = new System.Windows.Forms.ComboBox();
      this.comboBoxEthnicity = new System.Windows.Forms.ComboBox();
      this.comboBoxGender = new System.Windows.Forms.ComboBox();
      this.label11 = new System.Windows.Forms.Label();
      this.label5 = new System.Windows.Forms.Label();
      this.label4 = new System.Windows.Forms.Label();
      this.label7 = new System.Windows.Forms.Label();
      this.label9 = new System.Windows.Forms.Label();
      this.label3 = new System.Windows.Forms.Label();
      this.label10 = new System.Windows.Forms.Label();
      this.label2 = new System.Windows.Forms.Label();
      this.textBoxOrderID = new System.Windows.Forms.TextBox();
      this.label6 = new System.Windows.Forms.Label();
      this.label1 = new System.Windows.Forms.Label();
      this.textBoxFirstName = new System.Windows.Forms.TextBox();
      this.textBoxLastname = new System.Windows.Forms.TextBox();
      this.textBoxPatientID = new System.Windows.Forms.TextBox();
      this.buttonAddWorklist = new System.Windows.Forms.Button();
      this.buttonSyncPatient = new System.Windows.Forms.Button();
      this.tabControlMain = new System.Windows.Forms.TabControl();
      this.tabPageSettings = new System.Windows.Forms.TabPage();
      this.label14 = new System.Windows.Forms.Label();
      this.label15 = new System.Windows.Forms.Label();
      this.label13 = new System.Windows.Forms.Label();
      this.comboBox4 = new System.Windows.Forms.ComboBox();
      this.comboBox3 = new System.Windows.Forms.ComboBox();
      this.comboBox2 = new System.Windows.Forms.ComboBox();
      this.comboBox1 = new System.Windows.Forms.ComboBox();
      this.groupBox1 = new System.Windows.Forms.GroupBox();
      this.textBoxEchangeFolder = new System.Windows.Forms.TextBox();
      this.label12 = new System.Windows.Forms.Label();
      this.checkBoxCloseAfterTest = new System.Windows.Forms.CheckBox();
      this.checkBoxIncludeTrialValues = new System.Windows.Forms.CheckBox();
      this.checkBoxIncludeCurveData = new System.Windows.Forms.CheckBox();
      this.label8 = new System.Windows.Forms.Label();
      this.checkBoxAttachPDF = new System.Windows.Forms.CheckBox();
      this.labelVersionInfo = new System.Windows.Forms.Label();
      this.tabPageEmrSetup.SuspendLayout();
      this.tabPageEmrMessage.SuspendLayout();
      this.panel1.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.numericUpDownWeight)).BeginInit();
      this.tabControlMain.SuspendLayout();
      this.tabPageSettings.SuspendLayout();
      this.groupBox1.SuspendLayout();
      this.SuspendLayout();
      // 
      // tabPageEmrSetup
      // 
      this.tabPageEmrSetup.Controls.Add(this.label16);
      this.tabPageEmrSetup.Controls.Add(this.checkBoxSupportSearchPatient);
      this.tabPageEmrSetup.Controls.Add(this.checkBoxSupportWorklist);
      this.tabPageEmrSetup.Location = new System.Drawing.Point(4, 22);
      this.tabPageEmrSetup.Name = "tabPageEmrSetup";
      this.tabPageEmrSetup.Padding = new System.Windows.Forms.Padding(3);
      this.tabPageEmrSetup.Size = new System.Drawing.Size(627, 580);
      this.tabPageEmrSetup.TabIndex = 1;
      this.tabPageEmrSetup.Text = "Supported Features";
      this.tabPageEmrSetup.UseVisualStyleBackColor = true;
      // 
      // label16
      // 
      this.label16.AutoSize = true;
      this.label16.Location = new System.Drawing.Point(16, 20);
      this.label16.Name = "label16";
      this.label16.Size = new System.Drawing.Size(202, 13);
      this.label16.TabIndex = 1;
      this.label16.Text = "Changed values are active after a restart.";
      // 
      // checkBoxSupportSearchPatient
      // 
      this.checkBoxSupportSearchPatient.AutoSize = true;
      this.checkBoxSupportSearchPatient.Checked = global::EMR.PlugIn.TestBench.Properties.Settings.Default.Supported_SearchPatients;
      this.checkBoxSupportSearchPatient.CheckState = System.Windows.Forms.CheckState.Checked;
      this.checkBoxSupportSearchPatient.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::EMR.PlugIn.TestBench.Properties.Settings.Default, "Supported_SearchPatients", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
      this.checkBoxSupportSearchPatient.Location = new System.Drawing.Point(19, 81);
      this.checkBoxSupportSearchPatient.Name = "checkBoxSupportSearchPatient";
      this.checkBoxSupportSearchPatient.Size = new System.Drawing.Size(101, 17);
      this.checkBoxSupportSearchPatient.TabIndex = 0;
      this.checkBoxSupportSearchPatient.Text = "Search Patients";
      this.checkBoxSupportSearchPatient.UseVisualStyleBackColor = true;
      // 
      // checkBoxSupportWorklist
      // 
      this.checkBoxSupportWorklist.AutoSize = true;
      this.checkBoxSupportWorklist.Checked = global::EMR.PlugIn.TestBench.Properties.Settings.Default.Supported_Worklist;
      this.checkBoxSupportWorklist.CheckState = System.Windows.Forms.CheckState.Checked;
      this.checkBoxSupportWorklist.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::EMR.PlugIn.TestBench.Properties.Settings.Default, "Supported_Worklist", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
      this.checkBoxSupportWorklist.Location = new System.Drawing.Point(19, 58);
      this.checkBoxSupportWorklist.Name = "checkBoxSupportWorklist";
      this.checkBoxSupportWorklist.Size = new System.Drawing.Size(64, 17);
      this.checkBoxSupportWorklist.TabIndex = 0;
      this.checkBoxSupportWorklist.Text = "Worklist";
      this.checkBoxSupportWorklist.UseVisualStyleBackColor = true;
      // 
      // tabPageEmrMessage
      // 
      this.tabPageEmrMessage.Controls.Add(this.panel1);
      this.tabPageEmrMessage.Controls.Add(this.button1);
      this.tabPageEmrMessage.Controls.Add(this.buttonPrintPreview);
      this.tabPageEmrMessage.Controls.Add(this.maskedTextBoxHeight);
      this.tabPageEmrMessage.Controls.Add(this.numericUpDownWeight);
      this.tabPageEmrMessage.Controls.Add(this.buttonGetTest);
      this.tabPageEmrMessage.Controls.Add(this.buttonShowTest);
      this.tabPageEmrMessage.Controls.Add(this.buttonPerformTestType);
      this.tabPageEmrMessage.Controls.Add(this.buttonPerformTest);
      this.tabPageEmrMessage.Controls.Add(this.richTextBoxReceivedMessages);
      this.tabPageEmrMessage.Controls.Add(this.dateTimePicker1);
      this.tabPageEmrMessage.Controls.Add(this.comboBoxTestType);
      this.tabPageEmrMessage.Controls.Add(this.comboBoxEthnicity);
      this.tabPageEmrMessage.Controls.Add(this.comboBoxGender);
      this.tabPageEmrMessage.Controls.Add(this.label11);
      this.tabPageEmrMessage.Controls.Add(this.label5);
      this.tabPageEmrMessage.Controls.Add(this.label4);
      this.tabPageEmrMessage.Controls.Add(this.label7);
      this.tabPageEmrMessage.Controls.Add(this.label9);
      this.tabPageEmrMessage.Controls.Add(this.label3);
      this.tabPageEmrMessage.Controls.Add(this.label10);
      this.tabPageEmrMessage.Controls.Add(this.label2);
      this.tabPageEmrMessage.Controls.Add(this.textBoxOrderID);
      this.tabPageEmrMessage.Controls.Add(this.label6);
      this.tabPageEmrMessage.Controls.Add(this.label1);
      this.tabPageEmrMessage.Controls.Add(this.textBoxFirstName);
      this.tabPageEmrMessage.Controls.Add(this.textBoxLastname);
      this.tabPageEmrMessage.Controls.Add(this.textBoxPatientID);
      this.tabPageEmrMessage.Controls.Add(this.buttonAddWorklist);
      this.tabPageEmrMessage.Controls.Add(this.buttonSyncPatient);
      this.tabPageEmrMessage.Location = new System.Drawing.Point(4, 22);
      this.tabPageEmrMessage.Name = "tabPageEmrMessage";
      this.tabPageEmrMessage.Padding = new System.Windows.Forms.Padding(3);
      this.tabPageEmrMessage.Size = new System.Drawing.Size(627, 580);
      this.tabPageEmrMessage.TabIndex = 0;
      this.tabPageEmrMessage.Text = "Messages";
      this.tabPageEmrMessage.UseVisualStyleBackColor = true;
      // 
      // panel1
      // 
      this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
      this.panel1.Controls.Add(this.label17);
      this.panel1.Controls.Add(this.textBoxTestTime);
      this.panel1.Location = new System.Drawing.Point(429, 127);
      this.panel1.Name = "panel1";
      this.panel1.Size = new System.Drawing.Size(190, 107);
      this.panel1.TabIndex = 41;
      // 
      // label17
      // 
      this.label17.AutoSize = true;
      this.label17.Location = new System.Drawing.Point(12, 13);
      this.label17.Name = "label17";
      this.label17.Size = new System.Drawing.Size(54, 13);
      this.label17.TabIndex = 31;
      this.label17.Text = "Test Time";
      // 
      // textBoxTestTime
      // 
      this.textBoxTestTime.Location = new System.Drawing.Point(72, 10);
      this.textBoxTestTime.Name = "textBoxTestTime";
      this.textBoxTestTime.Size = new System.Drawing.Size(111, 20);
      this.textBoxTestTime.TabIndex = 24;
      // 
      // button1
      // 
      this.button1.Location = new System.Drawing.Point(333, 212);
      this.button1.Name = "button1";
      this.button1.Size = new System.Drawing.Size(89, 23);
      this.button1.TabIndex = 40;
      this.button1.Text = "Print Test";
      this.button1.UseVisualStyleBackColor = true;
      this.button1.Click += new System.EventHandler(this.buttonPrint_Click);
      // 
      // buttonPrintPreview
      // 
      this.buttonPrintPreview.Location = new System.Drawing.Point(333, 183);
      this.buttonPrintPreview.Name = "buttonPrintPreview";
      this.buttonPrintPreview.Size = new System.Drawing.Size(89, 23);
      this.buttonPrintPreview.TabIndex = 40;
      this.buttonPrintPreview.Text = "Print Preview";
      this.buttonPrintPreview.UseVisualStyleBackColor = true;
      this.buttonPrintPreview.Click += new System.EventHandler(this.buttonPrintPreview_Click);
      // 
      // maskedTextBoxHeight
      // 
      this.maskedTextBoxHeight.Location = new System.Drawing.Point(84, 141);
      this.maskedTextBoxHeight.Mask = "#.##";
      this.maskedTextBoxHeight.Name = "maskedTextBoxHeight";
      this.maskedTextBoxHeight.Size = new System.Drawing.Size(163, 20);
      this.maskedTextBoxHeight.TabIndex = 37;
      this.maskedTextBoxHeight.Text = "170";
      // 
      // numericUpDownWeight
      // 
      this.numericUpDownWeight.Location = new System.Drawing.Point(84, 167);
      this.numericUpDownWeight.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
      this.numericUpDownWeight.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
      this.numericUpDownWeight.Name = "numericUpDownWeight";
      this.numericUpDownWeight.Size = new System.Drawing.Size(163, 20);
      this.numericUpDownWeight.TabIndex = 36;
      this.numericUpDownWeight.Value = new decimal(new int[] {
            70,
            0,
            0,
            0});
      // 
      // buttonGetTest
      // 
      this.buttonGetTest.Location = new System.Drawing.Point(333, 154);
      this.buttonGetTest.Name = "buttonGetTest";
      this.buttonGetTest.Size = new System.Drawing.Size(89, 23);
      this.buttonGetTest.TabIndex = 35;
      this.buttonGetTest.Text = "Get Test";
      this.buttonGetTest.UseVisualStyleBackColor = true;
      this.buttonGetTest.Click += new System.EventHandler(this.buttonGetTest_Click);
      // 
      // buttonShowTest
      // 
      this.buttonShowTest.Location = new System.Drawing.Point(333, 125);
      this.buttonShowTest.Name = "buttonShowTest";
      this.buttonShowTest.Size = new System.Drawing.Size(89, 23);
      this.buttonShowTest.TabIndex = 35;
      this.buttonShowTest.Text = "Show Test";
      this.buttonShowTest.UseVisualStyleBackColor = true;
      this.buttonShowTest.Click += new System.EventHandler(this.buttonShowTest_Click);
      // 
      // buttonPerformTestType
      // 
      this.buttonPerformTestType.Location = new System.Drawing.Point(333, 96);
      this.buttonPerformTestType.Name = "buttonPerformTestType";
      this.buttonPerformTestType.Size = new System.Drawing.Size(89, 23);
      this.buttonPerformTestType.TabIndex = 35;
      this.buttonPerformTestType.Text = "PerformTest";
      this.buttonPerformTestType.UseVisualStyleBackColor = true;
      this.buttonPerformTestType.Click += new System.EventHandler(this.buttonPerformTestType_Click);
      // 
      // buttonPerformTest
      // 
      this.buttonPerformTest.Location = new System.Drawing.Point(333, 67);
      this.buttonPerformTest.Name = "buttonPerformTest";
      this.buttonPerformTest.Size = new System.Drawing.Size(89, 23);
      this.buttonPerformTest.TabIndex = 35;
      this.buttonPerformTest.Text = "PerformTest";
      this.buttonPerformTest.UseVisualStyleBackColor = true;
      this.buttonPerformTest.Click += new System.EventHandler(this.buttonPerformTest_Click);
      // 
      // richTextBoxReceivedMessages
      // 
      this.richTextBoxReceivedMessages.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
      this.richTextBoxReceivedMessages.Location = new System.Drawing.Point(8, 293);
      this.richTextBoxReceivedMessages.Name = "richTextBoxReceivedMessages";
      this.richTextBoxReceivedMessages.Size = new System.Drawing.Size(611, 283);
      this.richTextBoxReceivedMessages.TabIndex = 34;
      this.richTextBoxReceivedMessages.Text = "-";
      // 
      // dateTimePicker1
      // 
      this.dateTimePicker1.Format = System.Windows.Forms.DateTimePickerFormat.Short;
      this.dateTimePicker1.Location = new System.Drawing.Point(84, 115);
      this.dateTimePicker1.Name = "dateTimePicker1";
      this.dateTimePicker1.Size = new System.Drawing.Size(163, 20);
      this.dateTimePicker1.TabIndex = 33;
      // 
      // comboBoxTestType
      // 
      this.comboBoxTestType.FormattingEnabled = true;
      this.comboBoxTestType.Items.AddRange(new object[] {
            "FVC",
            "FVL",
            "SVC",
            "MVV",
            "DLCO",
            "Cal"});
      this.comboBoxTestType.Location = new System.Drawing.Point(428, 96);
      this.comboBoxTestType.Name = "comboBoxTestType";
      this.comboBoxTestType.Size = new System.Drawing.Size(111, 21);
      this.comboBoxTestType.TabIndex = 32;
      this.comboBoxTestType.Text = "FVC";
      // 
      // comboBoxEthnicity
      // 
      this.comboBoxEthnicity.FormattingEnabled = true;
      this.comboBoxEthnicity.Items.AddRange(new object[] {
            "Caucasian",
            "Asian",
            "African",
            "Other_Ethnic",
            "Hispanic"});
      this.comboBoxEthnicity.Location = new System.Drawing.Point(84, 193);
      this.comboBoxEthnicity.Name = "comboBoxEthnicity";
      this.comboBoxEthnicity.Size = new System.Drawing.Size(163, 21);
      this.comboBoxEthnicity.TabIndex = 32;
      this.comboBoxEthnicity.Text = "Caucasian";
      // 
      // comboBoxGender
      // 
      this.comboBoxGender.FormattingEnabled = true;
      this.comboBoxGender.Items.AddRange(new object[] {
            "Female",
            "Male"});
      this.comboBoxGender.Location = new System.Drawing.Point(84, 88);
      this.comboBoxGender.Name = "comboBoxGender";
      this.comboBoxGender.Size = new System.Drawing.Size(163, 21);
      this.comboBoxGender.TabIndex = 32;
      this.comboBoxGender.Text = "Female";
      // 
      // label11
      // 
      this.label11.AutoSize = true;
      this.label11.Location = new System.Drawing.Point(11, 196);
      this.label11.Name = "label11";
      this.label11.Size = new System.Drawing.Size(47, 13);
      this.label11.TabIndex = 28;
      this.label11.Text = "Ethnicity";
      // 
      // label5
      // 
      this.label5.AutoSize = true;
      this.label5.Location = new System.Drawing.Point(11, 119);
      this.label5.Name = "label5";
      this.label5.Size = new System.Drawing.Size(66, 13);
      this.label5.TabIndex = 29;
      this.label5.Text = "Date of Birth";
      // 
      // label4
      // 
      this.label4.AutoSize = true;
      this.label4.Location = new System.Drawing.Point(11, 91);
      this.label4.Name = "label4";
      this.label4.Size = new System.Drawing.Size(42, 13);
      this.label4.TabIndex = 28;
      this.label4.Text = "Gender";
      // 
      // label7
      // 
      this.label7.AutoSize = true;
      this.label7.Location = new System.Drawing.Point(11, 246);
      this.label7.Name = "label7";
      this.label7.Size = new System.Drawing.Size(47, 13);
      this.label7.TabIndex = 31;
      this.label7.Text = "Order ID";
      // 
      // label9
      // 
      this.label9.AutoSize = true;
      this.label9.Location = new System.Drawing.Point(11, 171);
      this.label9.Name = "label9";
      this.label9.Size = new System.Drawing.Size(62, 13);
      this.label9.TabIndex = 31;
      this.label9.Text = "Weight [kg]";
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Location = new System.Drawing.Point(11, 144);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(55, 13);
      this.label3.TabIndex = 31;
      this.label3.Text = "Height [m]";
      // 
      // label10
      // 
      this.label10.AutoSize = true;
      this.label10.Location = new System.Drawing.Point(11, 65);
      this.label10.Name = "label10";
      this.label10.Size = new System.Drawing.Size(52, 13);
      this.label10.TabIndex = 30;
      this.label10.Text = "Firstname";
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(10, 40);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(53, 13);
      this.label2.TabIndex = 30;
      this.label2.Text = "Lastname";
      // 
      // textBoxOrderID
      // 
      this.textBoxOrderID.Location = new System.Drawing.Point(84, 243);
      this.textBoxOrderID.Name = "textBoxOrderID";
      this.textBoxOrderID.Size = new System.Drawing.Size(163, 20);
      this.textBoxOrderID.TabIndex = 23;
      this.textBoxOrderID.Text = "EMR-Order-177";
      // 
      // label6
      // 
      this.label6.AutoSize = true;
      this.label6.Location = new System.Drawing.Point(11, 277);
      this.label6.Name = "label6";
      this.label6.Size = new System.Drawing.Size(142, 13);
      this.label6.TabIndex = 27;
      this.label6.Text = "Returned from EasyWarePro";
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(11, 14);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(54, 13);
      this.label1.TabIndex = 26;
      this.label1.Text = "Patient ID";
      // 
      // textBoxFirstName
      // 
      this.textBoxFirstName.Location = new System.Drawing.Point(84, 62);
      this.textBoxFirstName.Name = "textBoxFirstName";
      this.textBoxFirstName.Size = new System.Drawing.Size(163, 20);
      this.textBoxFirstName.TabIndex = 24;
      this.textBoxFirstName.Text = "My firstname";
      // 
      // textBoxLastname
      // 
      this.textBoxLastname.Location = new System.Drawing.Point(84, 37);
      this.textBoxLastname.Name = "textBoxLastname";
      this.textBoxLastname.Size = new System.Drawing.Size(163, 20);
      this.textBoxLastname.TabIndex = 24;
      this.textBoxLastname.Text = "My lastname";
      // 
      // textBoxPatientID
      // 
      this.textBoxPatientID.Location = new System.Drawing.Point(84, 11);
      this.textBoxPatientID.Name = "textBoxPatientID";
      this.textBoxPatientID.Size = new System.Drawing.Size(163, 20);
      this.textBoxPatientID.TabIndex = 25;
      this.textBoxPatientID.Text = "EMR-177";
      // 
      // buttonAddWorklist
      // 
      this.buttonAddWorklist.Location = new System.Drawing.Point(333, 38);
      this.buttonAddWorklist.Name = "buttonAddWorklist";
      this.buttonAddWorklist.Size = new System.Drawing.Size(89, 23);
      this.buttonAddWorklist.TabIndex = 22;
      this.buttonAddWorklist.Text = "Add to Worklist";
      this.buttonAddWorklist.UseVisualStyleBackColor = true;
      this.buttonAddWorklist.Click += new System.EventHandler(this.buttonAddWorklist_Click);
      // 
      // buttonSyncPatient
      // 
      this.buttonSyncPatient.Location = new System.Drawing.Point(333, 9);
      this.buttonSyncPatient.Name = "buttonSyncPatient";
      this.buttonSyncPatient.Size = new System.Drawing.Size(89, 23);
      this.buttonSyncPatient.TabIndex = 21;
      this.buttonSyncPatient.Text = "Sync Patient";
      this.buttonSyncPatient.UseVisualStyleBackColor = true;
      this.buttonSyncPatient.Click += new System.EventHandler(this.buttonAddPatient_Click);
      // 
      // tabControlMain
      // 
      this.tabControlMain.Controls.Add(this.tabPageEmrMessage);
      this.tabControlMain.Controls.Add(this.tabPageEmrSetup);
      this.tabControlMain.Controls.Add(this.tabPageSettings);
      this.tabControlMain.Dock = System.Windows.Forms.DockStyle.Fill;
      this.tabControlMain.Location = new System.Drawing.Point(0, 0);
      this.tabControlMain.Name = "tabControlMain";
      this.tabControlMain.SelectedIndex = 0;
      this.tabControlMain.Size = new System.Drawing.Size(635, 606);
      this.tabControlMain.TabIndex = 21;
      // 
      // tabPageSettings
      // 
      this.tabPageSettings.Controls.Add(this.labelVersionInfo);
      this.tabPageSettings.Controls.Add(this.label14);
      this.tabPageSettings.Controls.Add(this.label15);
      this.tabPageSettings.Controls.Add(this.label13);
      this.tabPageSettings.Controls.Add(this.comboBox4);
      this.tabPageSettings.Controls.Add(this.comboBox3);
      this.tabPageSettings.Controls.Add(this.comboBox2);
      this.tabPageSettings.Controls.Add(this.comboBox1);
      this.tabPageSettings.Controls.Add(this.groupBox1);
      this.tabPageSettings.Controls.Add(this.label8);
      this.tabPageSettings.Controls.Add(this.checkBoxAttachPDF);
      this.tabPageSettings.Location = new System.Drawing.Point(4, 22);
      this.tabPageSettings.Name = "tabPageSettings";
      this.tabPageSettings.Padding = new System.Windows.Forms.Padding(3);
      this.tabPageSettings.Size = new System.Drawing.Size(627, 580);
      this.tabPageSettings.TabIndex = 2;
      this.tabPageSettings.Text = "Settings";
      this.tabPageSettings.UseVisualStyleBackColor = true;
      this.tabPageSettings.Enter += new System.EventHandler(this.tabPageSettings_Enter);
      // 
      // label14
      // 
      this.label14.AutoSize = true;
      this.label14.Location = new System.Drawing.Point(405, 130);
      this.label14.Name = "label14";
      this.label14.Size = new System.Drawing.Size(10, 13);
      this.label14.TabIndex = 1;
      this.label14.Text = ".";
      // 
      // label15
      // 
      this.label15.AutoSize = true;
      this.label15.Location = new System.Drawing.Point(418, 103);
      this.label15.Name = "label15";
      this.label15.Size = new System.Drawing.Size(93, 13);
      this.label15.TabIndex = 1;
      this.label15.Text = "Attachment format";
      // 
      // label13
      // 
      this.label13.AutoSize = true;
      this.label13.Location = new System.Drawing.Point(23, 103);
      this.label13.Name = "label13";
      this.label13.Size = new System.Drawing.Size(106, 13);
      this.label13.TabIndex = 1;
      this.label13.Text = "Attachment file name";
      // 
      // comboBox4
      // 
      this.comboBox4.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::EMR.PlugIn.TestBench.Properties.Settings.Default, "AttachmentFileNamePart3", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
      this.comboBox4.FormattingEnabled = true;
      this.comboBox4.Items.AddRange(new object[] {
            "%PatientID%",
            "%Firstname%",
            "%Lastname%",
            "%OrderID%"});
      this.comboBox4.Location = new System.Drawing.Point(278, 122);
      this.comboBox4.Name = "comboBox4";
      this.comboBox4.Size = new System.Drawing.Size(121, 21);
      this.comboBox4.TabIndex = 3;
      this.comboBox4.Text = global::EMR.PlugIn.TestBench.Properties.Settings.Default.AttachmentFileNamePart3;
      // 
      // comboBox3
      // 
      this.comboBox3.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::EMR.PlugIn.TestBench.Properties.Settings.Default, "AttachmentFileNamePart2", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
      this.comboBox3.FormattingEnabled = true;
      this.comboBox3.Items.AddRange(new object[] {
            "%PatientID%",
            "%Firstname%",
            "%Lastname%",
            "%OrderID%"});
      this.comboBox3.Location = new System.Drawing.Point(151, 122);
      this.comboBox3.Name = "comboBox3";
      this.comboBox3.Size = new System.Drawing.Size(121, 21);
      this.comboBox3.TabIndex = 3;
      this.comboBox3.Text = global::EMR.PlugIn.TestBench.Properties.Settings.Default.AttachmentFileNamePart2;
      // 
      // comboBox2
      // 
      this.comboBox2.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::EMR.PlugIn.TestBench.Properties.Settings.Default, "AttachmentFileNamePart1", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
      this.comboBox2.FormattingEnabled = true;
      this.comboBox2.Items.AddRange(new object[] {
            "%PatientID%",
            "%Firstname%",
            "%Lastname%",
            "%OrderID%"});
      this.comboBox2.Location = new System.Drawing.Point(24, 122);
      this.comboBox2.Name = "comboBox2";
      this.comboBox2.Size = new System.Drawing.Size(121, 21);
      this.comboBox2.TabIndex = 3;
      this.comboBox2.Text = global::EMR.PlugIn.TestBench.Properties.Settings.Default.AttachmentFileNamePart1;
      // 
      // comboBox1
      // 
      this.comboBox1.DataBindings.Add(new System.Windows.Forms.Binding("Text", global::EMR.PlugIn.TestBench.Properties.Settings.Default, "AttachmentFormat", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
      this.comboBox1.FormattingEnabled = true;
      this.comboBox1.Items.AddRange(new object[] {
            "PDF",
            "CSV",
            "TXT",
            "RTF",
            "XLS",
            "PNG",
            "GIF",
            "TIFF",
            "JPG",
            "MHT"});
      this.comboBox1.Location = new System.Drawing.Point(421, 122);
      this.comboBox1.Name = "comboBox1";
      this.comboBox1.Size = new System.Drawing.Size(121, 21);
      this.comboBox1.TabIndex = 3;
      this.comboBox1.Text = global::EMR.PlugIn.TestBench.Properties.Settings.Default.AttachmentFormat;
      // 
      // groupBox1
      // 
      this.groupBox1.Controls.Add(this.textBoxEchangeFolder);
      this.groupBox1.Controls.Add(this.label12);
      this.groupBox1.Controls.Add(this.checkBoxCloseAfterTest);
      this.groupBox1.Controls.Add(this.checkBoxIncludeTrialValues);
      this.groupBox1.Controls.Add(this.checkBoxIncludeCurveData);
      this.groupBox1.Enabled = false;
      this.groupBox1.Location = new System.Drawing.Point(8, 195);
      this.groupBox1.Name = "groupBox1";
      this.groupBox1.Size = new System.Drawing.Size(613, 215);
      this.groupBox1.TabIndex = 2;
      this.groupBox1.TabStop = false;
      this.groupBox1.Text = "Configured in base class (GUI in configuration form)";
      // 
      // textBoxEchangeFolder
      // 
      this.textBoxEchangeFolder.Location = new System.Drawing.Point(16, 163);
      this.textBoxEchangeFolder.Name = "textBoxEchangeFolder";
      this.textBoxEchangeFolder.Size = new System.Drawing.Size(588, 20);
      this.textBoxEchangeFolder.TabIndex = 1;
      // 
      // label12
      // 
      this.label12.AutoSize = true;
      this.label12.Location = new System.Drawing.Point(15, 147);
      this.label12.Name = "label12";
      this.label12.Size = new System.Drawing.Size(102, 13);
      this.label12.TabIndex = 1;
      this.label12.Text = "File exchange folder";
      // 
      // checkBoxCloseAfterTest
      // 
      this.checkBoxCloseAfterTest.AutoSize = true;
      this.checkBoxCloseAfterTest.Enabled = false;
      this.checkBoxCloseAfterTest.Location = new System.Drawing.Point(16, 39);
      this.checkBoxCloseAfterTest.Name = "checkBoxCloseAfterTest";
      this.checkBoxCloseAfterTest.Size = new System.Drawing.Size(101, 17);
      this.checkBoxCloseAfterTest.TabIndex = 0;
      this.checkBoxCloseAfterTest.Text = "Close After Test";
      this.checkBoxCloseAfterTest.UseVisualStyleBackColor = true;
      // 
      // checkBoxIncludeTrialValues
      // 
      this.checkBoxIncludeTrialValues.AutoSize = true;
      this.checkBoxIncludeTrialValues.Enabled = false;
      this.checkBoxIncludeTrialValues.Location = new System.Drawing.Point(16, 115);
      this.checkBoxIncludeTrialValues.Name = "checkBoxIncludeTrialValues";
      this.checkBoxIncludeTrialValues.Size = new System.Drawing.Size(118, 17);
      this.checkBoxIncludeTrialValues.TabIndex = 0;
      this.checkBoxIncludeTrialValues.Text = "Include Trial values";
      this.checkBoxIncludeTrialValues.UseVisualStyleBackColor = true;
      // 
      // checkBoxIncludeCurveData
      // 
      this.checkBoxIncludeCurveData.AutoSize = true;
      this.checkBoxIncludeCurveData.Enabled = false;
      this.checkBoxIncludeCurveData.Location = new System.Drawing.Point(16, 92);
      this.checkBoxIncludeCurveData.Name = "checkBoxIncludeCurveData";
      this.checkBoxIncludeCurveData.Size = new System.Drawing.Size(118, 17);
      this.checkBoxIncludeCurveData.TabIndex = 0;
      this.checkBoxIncludeCurveData.Text = "Include Curve Data";
      this.checkBoxIncludeCurveData.UseVisualStyleBackColor = true;
      // 
      // label8
      // 
      this.label8.AutoSize = true;
      this.label8.Location = new System.Drawing.Point(8, 27);
      this.label8.Name = "label8";
      this.label8.Size = new System.Drawing.Size(604, 13);
      this.label8.TabIndex = 1;
      this.label8.Text = "Example of parameters which are not in the configuration form (shown in EasyWareP" +
    "ro) and defined elsewhere (e.g. fix in code)";
      // 
      // checkBoxAttachPDF
      // 
      this.checkBoxAttachPDF.AutoSize = true;
      this.checkBoxAttachPDF.Checked = global::EMR.PlugIn.TestBench.Properties.Settings.Default.Config_AttachPDF;
      this.checkBoxAttachPDF.CheckState = System.Windows.Forms.CheckState.Checked;
      this.checkBoxAttachPDF.DataBindings.Add(new System.Windows.Forms.Binding("Checked", global::EMR.PlugIn.TestBench.Properties.Settings.Default, "Config_AttachPDF", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
      this.checkBoxAttachPDF.Location = new System.Drawing.Point(24, 72);
      this.checkBoxAttachPDF.Name = "checkBoxAttachPDF";
      this.checkBoxAttachPDF.Size = new System.Drawing.Size(81, 17);
      this.checkBoxAttachPDF.TabIndex = 0;
      this.checkBoxAttachPDF.Text = "Attach PDF";
      this.checkBoxAttachPDF.UseVisualStyleBackColor = true;
      // 
      // labelVersionInfo
      // 
      this.labelVersionInfo.AutoSize = true;
      this.labelVersionInfo.Location = new System.Drawing.Point(8, 424);
      this.labelVersionInfo.Name = "labelVersionInfo";
      this.labelVersionInfo.Size = new System.Drawing.Size(55, 13);
      this.labelVersionInfo.TabIndex = 4;
      this.labelVersionInfo.Text = "Version: X";
      // 
      // EMRTestForm
      // 
      this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
      this.ClientSize = new System.Drawing.Size(635, 606);
      this.Controls.Add(this.tabControlMain);
      this.Name = "EMRTestForm";
      this.Text = "TestBenchForm";
      this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.EMRTestForm_FormClosing);
      this.tabPageEmrSetup.ResumeLayout(false);
      this.tabPageEmrSetup.PerformLayout();
      this.tabPageEmrMessage.ResumeLayout(false);
      this.tabPageEmrMessage.PerformLayout();
      this.panel1.ResumeLayout(false);
      this.panel1.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this.numericUpDownWeight)).EndInit();
      this.tabControlMain.ResumeLayout(false);
      this.tabPageSettings.ResumeLayout(false);
      this.tabPageSettings.PerformLayout();
      this.groupBox1.ResumeLayout(false);
      this.groupBox1.PerformLayout();
      this.ResumeLayout(false);

    }

    #endregion

    private System.Windows.Forms.TabPage tabPageEmrSetup;
    private System.Windows.Forms.TabPage tabPageEmrMessage;
    private System.Windows.Forms.RichTextBox richTextBoxReceivedMessages;
    private System.Windows.Forms.DateTimePicker dateTimePicker1;
    private System.Windows.Forms.ComboBox comboBoxGender;
    private System.Windows.Forms.Label label5;
    private System.Windows.Forms.Label label4;
    private System.Windows.Forms.Label label3;
    private System.Windows.Forms.Label label2;
    private System.Windows.Forms.Label label6;
    private System.Windows.Forms.Label label1;
    private System.Windows.Forms.TextBox textBoxLastname;
    private System.Windows.Forms.TextBox textBoxPatientID;
    private System.Windows.Forms.Button buttonAddWorklist;
    private System.Windows.Forms.Button buttonSyncPatient;
    private System.Windows.Forms.TabControl tabControlMain;
    private System.Windows.Forms.CheckBox checkBoxSupportSearchPatient;
    private System.Windows.Forms.CheckBox checkBoxSupportWorklist;
    private System.Windows.Forms.Label label7;
    private System.Windows.Forms.TextBox textBoxOrderID;
    private System.Windows.Forms.TabPage tabPageSettings;
    private System.Windows.Forms.CheckBox checkBoxAttachPDF;
    private System.Windows.Forms.Label label8;
    private System.Windows.Forms.Button buttonPerformTest;
    private System.Windows.Forms.NumericUpDown numericUpDownWeight;
    private System.Windows.Forms.MaskedTextBox maskedTextBoxHeight;
    private System.Windows.Forms.Label label9;
    private System.Windows.Forms.Label label10;
    private System.Windows.Forms.TextBox textBoxFirstName;
    private System.Windows.Forms.ComboBox comboBoxEthnicity;
    private System.Windows.Forms.Label label11;
    private System.Windows.Forms.Button buttonPerformTestType;
    private System.Windows.Forms.ComboBox comboBoxTestType;
    private System.Windows.Forms.Button buttonShowTest;
    private System.Windows.Forms.CheckBox checkBoxIncludeCurveData;
    private System.Windows.Forms.CheckBox checkBoxCloseAfterTest;
    private System.Windows.Forms.GroupBox groupBox1;
    private System.Windows.Forms.TextBox textBoxEchangeFolder;
    private System.Windows.Forms.Label label12;
    private System.Windows.Forms.Label label14;
    private System.Windows.Forms.Label label15;
    private System.Windows.Forms.Label label13;
    private System.Windows.Forms.ComboBox comboBox2;
    private System.Windows.Forms.ComboBox comboBox1;
    private System.Windows.Forms.ComboBox comboBox4;
    private System.Windows.Forms.ComboBox comboBox3;
    private System.Windows.Forms.CheckBox checkBoxIncludeTrialValues;
    private System.Windows.Forms.Label label16;
    private System.Windows.Forms.Button buttonGetTest;
    private System.Windows.Forms.Label label17;
    private System.Windows.Forms.TextBox textBoxTestTime;
    private System.Windows.Forms.Button buttonPrintPreview;
    private System.Windows.Forms.Panel panel1;
    private System.Windows.Forms.Button button1;
    private System.Windows.Forms.Label labelVersionInfo;

  }
}