using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Windows.Forms;
using ndd.EMR.SharedConst;

namespace EMR.PlugIn.CmdLine
{


  public partial class CmdLineCfg : UserControl, IEmrConfigControlCmdLine
  {
    public CmdLineCfg()
    {
      InitializeComponent();
    }

    #region -- Configuration --
    public void SetGuiValues(Dictionary<string, string> configValues)
    {

      if (configValues.ContainsKey(Commands.Configuration.FileExchangeFolder))
        textBoxEchangeFolder.Text = configValues[Commands.Configuration.FileExchangeFolder];
      else
        textBoxEchangeFolder.Text = System.IO.Path.GetTempPath();

      if (configValues.ContainsKey(Commands.Configuration.IncludeCurveData))
      {
        bool bValue = false;
        if (Boolean.TryParse(configValues[Commands.Configuration.IncludeCurveData], out bValue))
          checkBoxIncludeCurveData.Checked = bValue;
      }

      if (configValues.ContainsKey(Commands.Configuration.IncludeTrialValues))
      {
        bool bValue = false;
        if (Boolean.TryParse(configValues[Commands.Configuration.IncludeTrialValues], out bValue))
          checkBoxIncludeTrialValues.Checked = bValue;
      }

      if (configValues.ContainsKey(Commands.Configuration.CloseAfterTest))
      {
        bool bValue = false;
        if (Boolean.TryParse(configValues[Commands.Configuration.CloseAfterTest], out bValue))
          checkBoxCloseAfterTest.Checked = bValue;
      }

      if (configValues.ContainsKey(Commands.Configuration.AttachReport))
      {
        bool bValue = false;
        if (Boolean.TryParse(configValues[Commands.Configuration.AttachReport], out bValue))
          checkBoxAttachPDF.Checked = bValue;
      }

      if (configValues.ContainsKey(Commands.Configuration.AttachmentFormat))
        comboBoxFormat.Text = configValues[Commands.Configuration.AttachmentFormat];

      #region - Attachment File Name -
      if (configValues.ContainsKey(UserConfig_Defs.ParamName_ExchangeFilePart1))
        AddAndSelectDropDownItem(configValues[UserConfig_Defs.ParamName_ExchangeFilePart1], comboBoxFileNamePart1);

      if (configValues.ContainsKey(UserConfig_Defs.ParamName_ExchangeFilePart2))
        AddAndSelectDropDownItem(configValues[UserConfig_Defs.ParamName_ExchangeFilePart1], comboBoxFileNamePart2);

      if (configValues.ContainsKey(UserConfig_Defs.ParamName_ExchangeFilePart3))
        AddAndSelectDropDownItem(configValues[UserConfig_Defs.ParamName_ExchangeFilePart1], comboBoxFileNamePart3);
      #endregion
    }


    /// <summary>
    /// Add item to combobox when not already listed. Then select the item.</summary>
    /// <param name="selectedCfg">current configuration value</param>
    /// <param name="comboBox">combobox which should contain the value</param>
    private static void AddAndSelectDropDownItem(string selectedCfg, ComboBox comboBox)
    {
      if (String.IsNullOrEmpty(selectedCfg))
        return;
      
      bool comboBoxContainsItem = false;
      for (int i = 0; i < comboBox.Items.Count; i++)
      {
        if (((string)comboBox.Items[i]) == selectedCfg)
        {
          comboBox.SelectedIndex = i;
          comboBoxContainsItem= true;
          break;
        }
      }
      if (!comboBoxContainsItem)
      {
        int addedIdx = comboBox.Items.Add(selectedCfg);
        comboBox.SelectedIndex = addedIdx;
      }
    }
 

    public bool ValidateEntries()
    {
      bool bNoneValidCfg = false;


      if (String.IsNullOrEmpty(textBoxEchangeFolder.Text) || !Directory.Exists(textBoxEchangeFolder.Text))
      {
        bNoneValidCfg = true;
        errorProviderValidation.SetError(textBoxEchangeFolder, "Field is empty or folder does not exist.");
      }
      else
        errorProviderValidation.SetError(textBoxEchangeFolder, "");

      return !bNoneValidCfg;
    }

    public Dictionary<string, string> GetGuiValues()
    {
      Dictionary<string, string> configValues = new Dictionary<string, string>();

      //configValues.Add(UserConfig_Defs.ParamName_ExchangeFile, textBox1.Text);
      //configValues.Add(UserConfig_Defs.ParamName_Password, textBoxPassword.Text);
      configValues.Add(Commands.Configuration.FileExchangeFolder, textBoxEchangeFolder.Text);
      configValues.Add(Commands.Configuration.IncludeCurveData, checkBoxIncludeCurveData.Checked.ToString(CultureInfo.InvariantCulture));
      configValues.Add(Commands.Configuration.IncludeTrialValues, checkBoxIncludeTrialValues.Checked.ToString(CultureInfo.InvariantCulture));
      configValues.Add(Commands.Configuration.CloseAfterTest, checkBoxCloseAfterTest.Checked.ToString(CultureInfo.InvariantCulture));
      configValues.Add(Commands.Configuration.AttachReport, checkBoxAttachPDF.Checked.ToString(CultureInfo.InvariantCulture));
      configValues.Add(Commands.Configuration.AttachmentFormat, comboBoxFormat.Text);
      configValues.Add(Commands.Configuration.AttachmentFileName,String.Format("{0}{1}{2}.{3}", comboBoxFileNamePart1.Text, comboBoxFileNamePart2.Text, comboBoxFileNamePart3.Text, comboBoxFormat.Text));
  
      //plugIn/custom specific values
      configValues.Add(UserConfig_Defs.ParamName_ExchangeFilePart1, comboBoxFileNamePart1.Text);
      configValues.Add(UserConfig_Defs.ParamName_ExchangeFilePart2, comboBoxFileNamePart2.Text);
      configValues.Add(UserConfig_Defs.ParamName_ExchangeFilePart3, comboBoxFileNamePart3.Text);

      return configValues;
    }
    #endregion

    private void buttonLogin_Click(object sender, EventArgs e)
    {
      if (textBox1.Text == "CmdLine")
        groupBoxCfg.Enabled = true;
    }

    private void textBox1_KeyDown(object sender, KeyEventArgs e)
    {
      if (e.KeyCode == Keys.Enter)
      {
        buttonLogin_Click(sender, e);
      }
    }

    //private void buttonSelectFolder_Click(object sender, EventArgs e)
    //{
    //  openFileDialogDataExchangeFile.FileName = textBox1.Text;
    //  if (openFileDialogDataExchangeFile.ShowDialog() == DialogResult.OK)
    //  {
    //    textBox1.Text = openFileDialogDataExchangeFile.FileName;
    //  }
    //}
  }

  public static class UserConfig_Defs
  {
    public static string ParamName_ExchangeFilePart1 = "ExchangeFilePart1";
    public static string ParamName_ExchangeFilePart2 = "ExchangeFilePart2";
    public static string ParamName_ExchangeFilePart3 = "ExchangeFilePart3";
    public static string ParamName_ExchangeFile      = "ExchangeFile";
  }
}
