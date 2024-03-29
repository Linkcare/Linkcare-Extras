﻿using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Windows.Forms;
using ndd.EMR.SharedConst;

namespace EMR.PlugIn.Basic
{


  public partial class UserControlEmrConfig : UserControl, IEmrConfigControl
  {
    public UserControlEmrConfig()
    {
      InitializeComponent();
    }

    public void SetGuiValues(Dictionary<string, string> configValues)
    {
      if (configValues.ContainsKey(UserConfig_Defs.ParamName_UseCmdFile))
      {
        bool bValue = false;
        if (Boolean.TryParse(configValues[UserConfig_Defs.ParamName_UseCmdFile], out bValue))
          checkBoxUseCmdFile.Checked = bValue;
      }

      if (configValues.ContainsKey(UserConfig_Defs.ParamName_CmdFileIn))
        textBoxCmdFileIn.Text = configValues[UserConfig_Defs.ParamName_CmdFileIn];

      if (configValues.ContainsKey(UserConfig_Defs.ParamName_CmdFileOut))
        textBoxCmdFileOut.Text = configValues[UserConfig_Defs.ParamName_CmdFileOut];

      if (configValues.ContainsKey(Commands.Configuration.FileExchangeFolder))
        textBoxExchangeFolder.Text = configValues[Commands.Configuration.FileExchangeFolder];

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


      if (configValues.ContainsKey(Commands.Configuration.SkipPatientSelectionConfirmation))
      {
        bool bValue = false;
        if (Boolean.TryParse(configValues[Commands.Configuration.SkipPatientSelectionConfirmation], out bValue))
          checkBoxSkipPatientConfirmation.Checked = bValue;
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
      if (configValues.ContainsKey(UserConfig_Defs.ParamName_AttachmentFilenamePart1))
        AddAndSelectDropDownItem(configValues[UserConfig_Defs.ParamName_AttachmentFilenamePart1], comboBoxFileNamePart1);

      if (configValues.ContainsKey(UserConfig_Defs.ParamName_AttachmentFilenamePart2))
        AddAndSelectDropDownItem(configValues[UserConfig_Defs.ParamName_AttachmentFilenamePart2], comboBoxFileNamePart2);

      if (configValues.ContainsKey(UserConfig_Defs.ParamName_AttachmentFilenamePart3))
        AddAndSelectDropDownItem(configValues[UserConfig_Defs.ParamName_AttachmentFilenamePart3], comboBoxFileNamePart3);
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
      if (String.IsNullOrEmpty(textBoxCmdFileIn.Text) && checkBoxUseCmdFile.Checked)
      {
        bNoneValidCfg = true;
        tabControlConfig.SelectedTab = tabPageExchange;
        errorProviderValidation.SetError(textBoxCmdFileIn, "Field is empty");
      }
      else
        errorProviderValidation.SetError(textBoxCmdFileIn, "");

      if (String.IsNullOrEmpty(textBoxCmdFileOut.Text) && checkBoxUseCmdFile.Checked)
      {
        bNoneValidCfg = true;
        tabControlConfig.SelectedTab = tabPageExchange;
        errorProviderValidation.SetError(textBoxCmdFileOut, "Field is empty");
      }
      else
        errorProviderValidation.SetError(textBoxCmdFileOut, "");

      if (String.IsNullOrEmpty(textBoxExchangeFolder.Text) || !Directory.Exists(textBoxExchangeFolder.Text))
      {
        bNoneValidCfg = true;
        tabControlConfig.SelectedTab = tabPageExchange;
        errorProviderValidation.SetError(textBoxExchangeFolder, "Field is empty or folder does not exist.");
      }
      else
        errorProviderValidation.SetError(textBoxExchangeFolder, "");

      return !bNoneValidCfg;
    }

    public Dictionary<string, string> GetGuiValues()
    {
      Dictionary<string, string> configValues = new Dictionary<string, string>();

      configValues.Add(UserConfig_Defs.ParamName_UseCmdFile, checkBoxUseCmdFile.Checked.ToString(CultureInfo.InvariantCulture));
      configValues.Add(UserConfig_Defs.ParamName_CmdFileIn, textBoxCmdFileIn.Text);
      configValues.Add(UserConfig_Defs.ParamName_CmdFileOut, textBoxCmdFileOut.Text);
      configValues.Add(Commands.Configuration.FileExchangeFolder, textBoxExchangeFolder.Text);
      configValues.Add(Commands.Configuration.IncludeCurveData,                 checkBoxIncludeCurveData       .Checked.ToString(CultureInfo.InvariantCulture));
      configValues.Add(Commands.Configuration.IncludeTrialValues,               checkBoxIncludeTrialValues.     Checked.ToString(CultureInfo.InvariantCulture));
      configValues.Add(Commands.Configuration.CloseAfterTest,                   checkBoxCloseAfterTest.         Checked.ToString(CultureInfo.InvariantCulture));
      configValues.Add(Commands.Configuration.SkipPatientSelectionConfirmation, checkBoxSkipPatientConfirmation.Checked.ToString(CultureInfo.InvariantCulture));

      configValues.Add(Commands.Configuration.AttachReport, checkBoxAttachPDF.Checked.ToString(CultureInfo.InvariantCulture));
      configValues.Add(Commands.Configuration.AttachmentFormat, comboBoxFormat.Text);
      configValues.Add(Commands.Configuration.AttachmentFileName, String.Format("{0}{1}{2}.{3}", comboBoxFileNamePart1.Text, comboBoxFileNamePart2.Text, comboBoxFileNamePart3.Text, comboBoxFormat.Text));

      //plugIn/custom specific values
      configValues.Add(UserConfig_Defs.ParamName_AttachmentFilenamePart1, comboBoxFileNamePart1.Text);
      configValues.Add(UserConfig_Defs.ParamName_AttachmentFilenamePart2, comboBoxFileNamePart2.Text);
      configValues.Add(UserConfig_Defs.ParamName_AttachmentFilenamePart3, comboBoxFileNamePart3.Text);
      return configValues;
    }

    private void checkBoxUseCmdFile_CheckedChanged(object sender, EventArgs e)
    {
      panelUseExternalCmdFile.Enabled = checkBoxUseCmdFile.Checked;
    }



  }

  public static class UserConfig_Defs
  {
    public static string ParamName_UseCmdFile = "UseCmdFile";
    public static string ParamName_CmdFileIn = "CmdFilenameIn";
    public static string ParamName_CmdFileOut = "CmdFileNameOut";
    public static string ParamName_AttachmentFilenamePart1 = "AttachmentFilenamePart1";
    public static string ParamName_AttachmentFilenamePart2 = "AttachmentFilenamePart2";
    public static string ParamName_AttachmentFilenamePart3 = "AttachmentFilenamePart3";
  }

}
