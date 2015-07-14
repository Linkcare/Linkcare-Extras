namespace EMR.PlugIn.Basic
{
    partial class UserControlEmrConfig
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
      this.tabControlConfig = new System.Windows.Forms.TabControl();
      this.tabPageExchange = new System.Windows.Forms.TabPage();
      this.checkBoxIncludeCurveData = new System.Windows.Forms.CheckBox();
      this.checkBoxIncludeTrialValues = new System.Windows.Forms.CheckBox();
      this.panelUseExternalCmdFile = new System.Windows.Forms.Panel();
      this.textBoxCmdFileIn = new System.Windows.Forms.TextBox();
      this.label1 = new System.Windows.Forms.Label();
      this.textBoxCmdFileOut = new System.Windows.Forms.TextBox();
      this.label2 = new System.Windows.Forms.Label();
      this.checkBoxUseCmdFile = new System.Windows.Forms.CheckBox();
      this.label3 = new System.Windows.Forms.Label();
      this.textBoxExchangeFolder = new System.Windows.Forms.TextBox();
      this.tabPageWorkflow = new System.Windows.Forms.TabPage();
      this.checkBoxBackgroundWorkflow = new System.Windows.Forms.CheckBox();
      this.checkBoxCloseAfterTest = new System.Windows.Forms.CheckBox();
      this.tabPageAttachment = new System.Windows.Forms.TabPage();
      this.label14 = new System.Windows.Forms.Label();
      this.label15 = new System.Windows.Forms.Label();
      this.label13 = new System.Windows.Forms.Label();
      this.comboBoxFileNamePart3 = new System.Windows.Forms.ComboBox();
      this.comboBoxFileNamePart2 = new System.Windows.Forms.ComboBox();
      this.comboBoxFileNamePart1 = new System.Windows.Forms.ComboBox();
      this.checkBoxAttachPDF = new System.Windows.Forms.CheckBox();
      this.comboBoxFormat = new System.Windows.Forms.ComboBox();
      this.errorProviderValidation = new System.Windows.Forms.ErrorProvider(this.components);
      this.checkBoxSkipPatientConfirmation = new System.Windows.Forms.CheckBox();
      this.tabControlConfig.SuspendLayout();
      this.tabPageExchange.SuspendLayout();
      this.panelUseExternalCmdFile.SuspendLayout();
      this.tabPageWorkflow.SuspendLayout();
      this.tabPageAttachment.SuspendLayout();
      ((System.ComponentModel.ISupportInitialize)(this.errorProviderValidation)).BeginInit();
      this.SuspendLayout();
      // 
      // tabControlConfig
      // 
      this.tabControlConfig.Controls.Add(this.tabPageExchange);
      this.tabControlConfig.Controls.Add(this.tabPageWorkflow);
      this.tabControlConfig.Controls.Add(this.tabPageAttachment);
      this.tabControlConfig.Location = new System.Drawing.Point(0, 0);
      this.tabControlConfig.Name = "tabControlConfig";
      this.tabControlConfig.SelectedIndex = 0;
      this.tabControlConfig.Size = new System.Drawing.Size(563, 308);
      this.tabControlConfig.TabIndex = 3;
      // 
      // tabPageExchange
      // 
      this.tabPageExchange.AutoScroll = true;
      this.tabPageExchange.BackColor = System.Drawing.SystemColors.Control;
      this.tabPageExchange.Controls.Add(this.checkBoxIncludeCurveData);
      this.tabPageExchange.Controls.Add(this.checkBoxIncludeTrialValues);
      this.tabPageExchange.Controls.Add(this.panelUseExternalCmdFile);
      this.tabPageExchange.Controls.Add(this.checkBoxUseCmdFile);
      this.tabPageExchange.Controls.Add(this.label3);
      this.tabPageExchange.Controls.Add(this.textBoxExchangeFolder);
      this.tabPageExchange.Location = new System.Drawing.Point(4, 22);
      this.tabPageExchange.Name = "tabPageExchange";
      this.tabPageExchange.Padding = new System.Windows.Forms.Padding(3);
      this.tabPageExchange.Size = new System.Drawing.Size(555, 282);
      this.tabPageExchange.TabIndex = 2;
      this.tabPageExchange.Text = "Exchange";
      // 
      // checkBoxIncludeCurveData
      // 
      this.checkBoxIncludeCurveData.AutoSize = true;
      this.checkBoxIncludeCurveData.Location = new System.Drawing.Point(17, 236);
      this.checkBoxIncludeCurveData.Name = "checkBoxIncludeCurveData";
      this.checkBoxIncludeCurveData.Size = new System.Drawing.Size(118, 17);
      this.checkBoxIncludeCurveData.TabIndex = 11;
      this.checkBoxIncludeCurveData.Text = "Include Curve Data";
      this.checkBoxIncludeCurveData.UseVisualStyleBackColor = true;
      // 
      // checkBoxIncludeTrialValues
      // 
      this.checkBoxIncludeTrialValues.AutoSize = true;
      this.checkBoxIncludeTrialValues.Location = new System.Drawing.Point(17, 259);
      this.checkBoxIncludeTrialValues.Name = "checkBoxIncludeTrialValues";
      this.checkBoxIncludeTrialValues.Size = new System.Drawing.Size(118, 17);
      this.checkBoxIncludeTrialValues.TabIndex = 10;
      this.checkBoxIncludeTrialValues.Text = "Include Trial values";
      this.checkBoxIncludeTrialValues.UseVisualStyleBackColor = true;
      // 
      // panelUseExternalCmdFile
      // 
      this.panelUseExternalCmdFile.BackColor = System.Drawing.Color.WhiteSmoke;
      this.panelUseExternalCmdFile.Controls.Add(this.textBoxCmdFileIn);
      this.panelUseExternalCmdFile.Controls.Add(this.label1);
      this.panelUseExternalCmdFile.Controls.Add(this.textBoxCmdFileOut);
      this.panelUseExternalCmdFile.Controls.Add(this.label2);
      this.panelUseExternalCmdFile.Enabled = false;
      this.panelUseExternalCmdFile.Location = new System.Drawing.Point(17, 100);
      this.panelUseExternalCmdFile.Name = "panelUseExternalCmdFile";
      this.panelUseExternalCmdFile.Size = new System.Drawing.Size(431, 122);
      this.panelUseExternalCmdFile.TabIndex = 8;
      // 
      // textBoxCmdFileIn
      // 
      this.textBoxCmdFileIn.Location = new System.Drawing.Point(16, 32);
      this.textBoxCmdFileIn.Name = "textBoxCmdFileIn";
      this.textBoxCmdFileIn.Size = new System.Drawing.Size(399, 20);
      this.textBoxCmdFileIn.TabIndex = 4;
      // 
      // label1
      // 
      this.label1.AutoSize = true;
      this.label1.Location = new System.Drawing.Point(13, 15);
      this.label1.Name = "label1";
      this.label1.Size = new System.Drawing.Size(61, 13);
      this.label1.TabIndex = 6;
      this.label1.Text = "Filename In";
      // 
      // textBoxCmdFileOut
      // 
      this.textBoxCmdFileOut.Location = new System.Drawing.Point(16, 81);
      this.textBoxCmdFileOut.Name = "textBoxCmdFileOut";
      this.textBoxCmdFileOut.Size = new System.Drawing.Size(399, 20);
      this.textBoxCmdFileOut.TabIndex = 5;
      // 
      // label2
      // 
      this.label2.AutoSize = true;
      this.label2.Location = new System.Drawing.Point(13, 64);
      this.label2.Name = "label2";
      this.label2.Size = new System.Drawing.Size(69, 13);
      this.label2.TabIndex = 7;
      this.label2.Text = "Filename Out";
      // 
      // checkBoxUseCmdFile
      // 
      this.checkBoxUseCmdFile.AutoSize = true;
      this.checkBoxUseCmdFile.Location = new System.Drawing.Point(17, 77);
      this.checkBoxUseCmdFile.Name = "checkBoxUseCmdFile";
      this.checkBoxUseCmdFile.Size = new System.Drawing.Size(126, 17);
      this.checkBoxUseCmdFile.TabIndex = 3;
      this.checkBoxUseCmdFile.Text = "use external Cmd File";
      this.checkBoxUseCmdFile.UseVisualStyleBackColor = true;
      this.checkBoxUseCmdFile.CheckedChanged += new System.EventHandler(this.checkBoxUseCmdFile_CheckedChanged);
      // 
      // label3
      // 
      this.label3.AutoSize = true;
      this.label3.Location = new System.Drawing.Point(14, 23);
      this.label3.Name = "label3";
      this.label3.Size = new System.Drawing.Size(109, 13);
      this.label3.TabIndex = 2;
      this.label3.Text = "Data exchange folder";
      // 
      // textBoxExchangeFolder
      // 
      this.textBoxExchangeFolder.Location = new System.Drawing.Point(17, 40);
      this.textBoxExchangeFolder.Name = "textBoxExchangeFolder";
      this.textBoxExchangeFolder.Size = new System.Drawing.Size(431, 20);
      this.textBoxExchangeFolder.TabIndex = 1;
      // 
      // tabPageWorkflow
      // 
      this.tabPageWorkflow.AutoScroll = true;
      this.tabPageWorkflow.BackColor = System.Drawing.SystemColors.Control;
      this.tabPageWorkflow.Controls.Add(this.checkBoxSkipPatientConfirmation);
      this.tabPageWorkflow.Controls.Add(this.checkBoxBackgroundWorkflow);
      this.tabPageWorkflow.Controls.Add(this.checkBoxCloseAfterTest);
      this.tabPageWorkflow.Location = new System.Drawing.Point(4, 22);
      this.tabPageWorkflow.Name = "tabPageWorkflow";
      this.tabPageWorkflow.Padding = new System.Windows.Forms.Padding(3);
      this.tabPageWorkflow.Size = new System.Drawing.Size(555, 282);
      this.tabPageWorkflow.TabIndex = 1;
      this.tabPageWorkflow.Text = "Workflow";
      // 
      // checkBoxBackgroundWorkflow
      // 
      this.checkBoxBackgroundWorkflow.AutoSize = true;
      this.checkBoxBackgroundWorkflow.Enabled = false;
      this.checkBoxBackgroundWorkflow.Location = new System.Drawing.Point(22, 60);
      this.checkBoxBackgroundWorkflow.Name = "checkBoxBackgroundWorkflow";
      this.checkBoxBackgroundWorkflow.Size = new System.Drawing.Size(226, 17);
      this.checkBoxBackgroundWorkflow.TabIndex = 3;
      this.checkBoxBackgroundWorkflow.Text = "Sync patient/order without user interaction";
      this.checkBoxBackgroundWorkflow.UseVisualStyleBackColor = true;
      // 
      // checkBoxCloseAfterTest
      // 
      this.checkBoxCloseAfterTest.AutoSize = true;
      this.checkBoxCloseAfterTest.Location = new System.Drawing.Point(22, 37);
      this.checkBoxCloseAfterTest.Name = "checkBoxCloseAfterTest";
      this.checkBoxCloseAfterTest.Size = new System.Drawing.Size(224, 17);
      this.checkBoxCloseAfterTest.TabIndex = 2;
      this.checkBoxCloseAfterTest.Text = "close application after test (return to EMR)";
      this.checkBoxCloseAfterTest.UseVisualStyleBackColor = true;
      // 
      // tabPageAttachment
      // 
      this.tabPageAttachment.BackColor = System.Drawing.SystemColors.Control;
      this.tabPageAttachment.Controls.Add(this.label14);
      this.tabPageAttachment.Controls.Add(this.label15);
      this.tabPageAttachment.Controls.Add(this.label13);
      this.tabPageAttachment.Controls.Add(this.comboBoxFileNamePart3);
      this.tabPageAttachment.Controls.Add(this.comboBoxFileNamePart2);
      this.tabPageAttachment.Controls.Add(this.comboBoxFileNamePart1);
      this.tabPageAttachment.Controls.Add(this.checkBoxAttachPDF);
      this.tabPageAttachment.Controls.Add(this.comboBoxFormat);
      this.tabPageAttachment.Location = new System.Drawing.Point(4, 22);
      this.tabPageAttachment.Name = "tabPageAttachment";
      this.tabPageAttachment.Padding = new System.Windows.Forms.Padding(3);
      this.tabPageAttachment.Size = new System.Drawing.Size(555, 282);
      this.tabPageAttachment.TabIndex = 3;
      this.tabPageAttachment.Text = "Attachment";
      // 
      // label14
      // 
      this.label14.AutoSize = true;
      this.label14.Location = new System.Drawing.Point(400, 158);
      this.label14.Name = "label14";
      this.label14.Size = new System.Drawing.Size(10, 13);
      this.label14.TabIndex = 14;
      this.label14.Text = ".";
      // 
      // label15
      // 
      this.label15.AutoSize = true;
      this.label15.Location = new System.Drawing.Point(413, 131);
      this.label15.Name = "label15";
      this.label15.Size = new System.Drawing.Size(93, 13);
      this.label15.TabIndex = 16;
      this.label15.Text = "Attachment format";
      // 
      // label13
      // 
      this.label13.AutoSize = true;
      this.label13.Location = new System.Drawing.Point(18, 131);
      this.label13.Name = "label13";
      this.label13.Size = new System.Drawing.Size(106, 13);
      this.label13.TabIndex = 15;
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
      this.comboBoxFileNamePart3.Location = new System.Drawing.Point(273, 150);
      this.comboBoxFileNamePart3.Name = "comboBoxFileNamePart3";
      this.comboBoxFileNamePart3.Size = new System.Drawing.Size(121, 21);
      this.comboBoxFileNamePart3.TabIndex = 19;
      // 
      // comboBoxFileNamePart2
      // 
      this.comboBoxFileNamePart2.FormattingEnabled = true;
      this.comboBoxFileNamePart2.Items.AddRange(new object[] {
            "%PatientID%",
            "%Firstname%",
            "%Lastname%",
            "%OrderID%"});
      this.comboBoxFileNamePart2.Location = new System.Drawing.Point(146, 150);
      this.comboBoxFileNamePart2.Name = "comboBoxFileNamePart2";
      this.comboBoxFileNamePart2.Size = new System.Drawing.Size(121, 21);
      this.comboBoxFileNamePart2.TabIndex = 17;
      // 
      // comboBoxFileNamePart1
      // 
      this.comboBoxFileNamePart1.FormattingEnabled = true;
      this.comboBoxFileNamePart1.Items.AddRange(new object[] {
            "%PatientID%",
            "%Firstname%",
            "%Lastname%",
            "%OrderID%"});
      this.comboBoxFileNamePart1.Location = new System.Drawing.Point(19, 150);
      this.comboBoxFileNamePart1.Name = "comboBoxFileNamePart1";
      this.comboBoxFileNamePart1.Size = new System.Drawing.Size(121, 21);
      this.comboBoxFileNamePart1.TabIndex = 20;
      this.comboBoxFileNamePart1.Text = "%PatientID%";
      // 
      // checkBoxAttachPDF
      // 
      this.checkBoxAttachPDF.AutoSize = true;
      this.checkBoxAttachPDF.Checked = true;
      this.checkBoxAttachPDF.CheckState = System.Windows.Forms.CheckState.Checked;
      this.checkBoxAttachPDF.Location = new System.Drawing.Point(19, 111);
      this.checkBoxAttachPDF.Name = "checkBoxAttachPDF";
      this.checkBoxAttachPDF.Size = new System.Drawing.Size(81, 17);
      this.checkBoxAttachPDF.TabIndex = 13;
      this.checkBoxAttachPDF.Text = "Attach PDF";
      this.checkBoxAttachPDF.UseVisualStyleBackColor = true;
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
      this.comboBoxFormat.Location = new System.Drawing.Point(416, 150);
      this.comboBoxFormat.Name = "comboBoxFormat";
      this.comboBoxFormat.Size = new System.Drawing.Size(121, 21);
      this.comboBoxFormat.TabIndex = 18;
      this.comboBoxFormat.Text = "PDF";
      // 
      // errorProviderValidation
      // 
      this.errorProviderValidation.ContainerControl = this;
      // 
      // checkBoxSkipPatientConfirmation
      // 
      this.checkBoxSkipPatientConfirmation.AutoSize = true;
      this.checkBoxSkipPatientConfirmation.Location = new System.Drawing.Point(22, 83);
      this.checkBoxSkipPatientConfirmation.Name = "checkBoxSkipPatientConfirmation";
      this.checkBoxSkipPatientConfirmation.Size = new System.Drawing.Size(144, 17);
      this.checkBoxSkipPatientConfirmation.TabIndex = 4;
      this.checkBoxSkipPatientConfirmation.Text = "Skip Patient Confirmation";
      this.checkBoxSkipPatientConfirmation.UseVisualStyleBackColor = true;
      // 
      // UserControlEmrConfig
      // 
      this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
      this.AutoScroll = true;
      this.AutoSize = true;
      this.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
      this.Controls.Add(this.tabControlConfig);
      this.Name = "UserControlEmrConfig";
      this.Size = new System.Drawing.Size(566, 311);
      this.tabControlConfig.ResumeLayout(false);
      this.tabPageExchange.ResumeLayout(false);
      this.tabPageExchange.PerformLayout();
      this.panelUseExternalCmdFile.ResumeLayout(false);
      this.panelUseExternalCmdFile.PerformLayout();
      this.tabPageWorkflow.ResumeLayout(false);
      this.tabPageWorkflow.PerformLayout();
      this.tabPageAttachment.ResumeLayout(false);
      this.tabPageAttachment.PerformLayout();
      ((System.ComponentModel.ISupportInitialize)(this.errorProviderValidation)).EndInit();
      this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControlConfig;
        private System.Windows.Forms.TabPage tabPageWorkflow;
        private System.Windows.Forms.ErrorProvider errorProviderValidation;
      private System.Windows.Forms.TabPage tabPageExchange;
      private System.Windows.Forms.Label label3;
      private System.Windows.Forms.TextBox textBoxExchangeFolder;
      private System.Windows.Forms.CheckBox checkBoxCloseAfterTest;
      private System.Windows.Forms.Panel panelUseExternalCmdFile;
      private System.Windows.Forms.TextBox textBoxCmdFileIn;
      private System.Windows.Forms.Label label1;
      private System.Windows.Forms.TextBox textBoxCmdFileOut;
      private System.Windows.Forms.Label label2;
      private System.Windows.Forms.CheckBox checkBoxUseCmdFile;
      private System.Windows.Forms.CheckBox checkBoxBackgroundWorkflow;
      private System.Windows.Forms.TabPage tabPageAttachment;
      private System.Windows.Forms.Label label14;
      private System.Windows.Forms.Label label15;
      private System.Windows.Forms.Label label13;
      private System.Windows.Forms.ComboBox comboBoxFileNamePart3;
      private System.Windows.Forms.ComboBox comboBoxFileNamePart2;
      private System.Windows.Forms.ComboBox comboBoxFileNamePart1;
      private System.Windows.Forms.CheckBox checkBoxAttachPDF;
      private System.Windows.Forms.ComboBox comboBoxFormat;
      private System.Windows.Forms.CheckBox checkBoxIncludeTrialValues;
      private System.Windows.Forms.CheckBox checkBoxIncludeCurveData;
      private System.Windows.Forms.CheckBox checkBoxSkipPatientConfirmation;
    }
}
