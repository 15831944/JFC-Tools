// Facility_Management.CreateUpdate
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Windows.Forms;
using Facility_Management;
using Facility_Management.My;
using ICSharpCodeAU.SharpZipLib.Zip;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;
using Facility_Management.Properties;

namespace Facility_Management
{

    public partial class CreateUpdate : Form
    {
        private string ProjectFile;

        private string MediaName;

        private string FileTxtInfoPatch;

        private string BuildLocation;

        private string BuildZipLocation;

        private string PackageLocation;

        private string Application;

        private string VersionFolder;

        private string FilterDirectory;

        private readonly string FilterFiles;

        private string[] ExcludedFiles;

        private string FilterVersion;

        private string Tarifs;

        private string TarifsFolder;

        private bool bMem_ButtonCreating;

        private bool bMem_ButtonGetData;

        private bool bMem_ButtonPigeAuto;

        private bool bMem_ButtonGetTarifs;

        private bool bMem_ButtonGetVersions;

        private bool bMem_ComboBox1;

        private bool bMem_TextBox2;

        [CompilerGenerated]
        internal sealed class PathVersion
        {
            // Fields
            public string Local_svSelectedVersion;

            // Methods
            public PathVersion()
            {
            }

            public PathVersion(CreateUpdate.PathVersion arg0)
            {
                if (arg0 != null)
                {
                    this.Local_svSelectedVersion = arg0.Local_svSelectedVersion;
                }
            }

            internal string GetPath(string w) =>
                    Strings.Replace(w, this.Local_svSelectedVersion, "", 1, -1, CompareMethod.Binary);
        }






        public CreateUpdate()
        {
            base.Load += Form1_Load;
            base.KeyDown += HandleKeyDown;
            InitializeComponent();
        }

        private void InitProjetInstallShield()
        {
            ref string menuSoft = ref MyProject.Forms.FormBox.MenuSoft;
            string MotCle = "ProjectFile";
            ProjectFile = mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft2 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "MediaName";
            MediaName = mFileIni.Select_GetIniString(ref menuSoft2, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft3 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "InfoUpdate";
            FileTxtInfoPatch = mFileIni.Select_GetIniString(ref menuSoft3, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft4 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "BuildLocation";
            BuildLocation = mFileIni.Select_GetIniString(ref menuSoft4, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft5 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "BuildZipLocation";
            BuildZipLocation = mFileIni.Select_GetIniString(ref menuSoft5, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft6 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "PackageLocation";
            PackageLocation = mFileIni.Select_GetIniString(ref menuSoft6, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft7 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "Application";
            Application = mFileIni.Select_GetIniString(ref menuSoft7, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft8 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "VersionFolder";
            VersionFolder = mFileIni.Select_GetIniString(ref menuSoft8, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft9 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "TarifsFolder";
            TarifsFolder = mFileIni.Select_GetIniString(ref menuSoft9, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft10 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "FilterDirectory";
            FilterDirectory = mFileIni.Select_GetIniString(ref menuSoft10, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft11 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "FilterVersion";
            FilterVersion = mFileIni.Select_GetIniString(ref menuSoft11, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft12 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "Tarifs";
            Tarifs = mFileIni.Select_GetIniString(ref menuSoft12, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft13 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "ExcludedFiles";
            ExcludedFiles = Strings.Split(mFileIni.Select_GetIniString(ref menuSoft13, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim(), ";");
            Button buttonPigeAuto = ButtonPigeAuto;
            ref string menuSoft14 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "Pigedata";
            buttonPigeAuto.Enabled = (Operators.CompareString(mFileIni.Select_GetIniString(ref menuSoft14, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim(), "", TextCompare: false) != 0) & (Operators.CompareString(MyProject.Forms.FormBox.svAppBuild, null, TextCompare: false) != 0);
            ButtonGetTarifs.Enabled = Operators.CompareString(Tarifs, "", TextCompare: false) != 0;
            Button buttonGetData = ButtonGetData;
            ref string menuSoft15 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "Source";
            buttonGetData.Enabled = Operators.CompareString(mFileIni.Select_GetIniString(ref menuSoft15, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim(), "", TextCompare: false) != 0;
        }

        private void CheckFolderEmpty()
        {
            int num = 1;
            string text = "";
            ref string menuSoft = ref MyProject.Forms.FormBox.MenuSoft;
            string MotCle = "CheckFolderEmpty" + num.ToString();
            string text2 = mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            checked
            {
                while (text2 != "")
                {
                    if (Directory.Exists(text2))
                    {
                        int num2 = Directory.GetFiles(text2, "*.*", SearchOption.AllDirectories).Length;
                        if (num2 > 0)
                        {
                            text = text + "\r\n - " + mFunction.ParsePath(text2, 4) + " ( " + num2.ToString() + " files )";
                        }
                    }
                    else
                    {
                        Directory.CreateDirectory(text2);
                    }
                    num++;
                    ref string menuSoft2 = ref MyProject.Forms.FormBox.MenuSoft;
                    MotCle = "CheckFolderEmpty" + num.ToString();
                    text2 = mFileIni.Select_GetIniString(ref menuSoft2, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
                }
                if (Operators.CompareString(text, "", TextCompare: false) != 0 && Interaction.MsgBox("List of folders that are not empty." + text + "\r\n\r\nWould you erase all data?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo, "Clear Data") == MsgBoxResult.Yes)
                {
                    num = 1;
                    ref string menuSoft3 = ref MyProject.Forms.FormBox.MenuSoft;
                    MotCle = "CheckFolderEmpty" + num.ToString();
                    text2 = mFileIni.Select_GetIniString(ref menuSoft3, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
                    while (text2 != "")
                    {
                        string[] files = Directory.GetFiles(text2, "*.*", SearchOption.AllDirectories);
                        foreach (string text3 in files)
                        {
                            try
                            {
                                if (File.GetAttributes(text3) == (FileAttributes.ReadOnly | FileAttributes.Archive))
                                {
                                    File.SetAttributes(text3, FileAttributes.Archive);
                                }
                                File.Delete(text3);
                            }
                            catch (Exception ex)
                            {
                                ProjectData.SetProjectError(ex);
                                Exception ex2 = ex;
                                Interaction.MsgBox(ex2.Message + "\r\n\r\n" + text3, MsgBoxStyle.Critical);
                                ProjectData.ClearProjectError();
                            }
                        }
                        num++;
                        ref string menuSoft4 = ref MyProject.Forms.FormBox.MenuSoft;
                        MotCle = "CheckFolderEmpty" + Conversion.Str(num).Trim();
                        text2 = mFileIni.Select_GetIniString(ref menuSoft4, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
                    }
                }
                num = 1;
                ref string menuSoft5 = ref MyProject.Forms.FormBox.MenuSoft;
                MotCle = "CheckFolderEmpty" + Conversion.Str(num).Trim();
                text2 = mFileIni.Select_GetIniString(ref menuSoft5, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
                while (Operators.CompareString(text2, "", TextCompare: false) != 0)
                {
                    try
                    {
                        string[] directories = Directory.GetDirectories(text2, "*.*", SearchOption.AllDirectories);
                        foreach (string text4 in directories)
                        {
                            if ((Directory.GetFiles(text4, "*.*", SearchOption.AllDirectories).Length == 0) & (Strings.StrComp(text4, text2, CompareMethod.Text) != 0))
                            {
                                Directory.Delete(text4, recursive: true);
                            }
                        }
                        num++;
                        ref string menuSoft6 = ref MyProject.Forms.FormBox.MenuSoft;
                        MotCle = "CheckFolderEmpty" + Conversion.Str(num).Trim();
                        text2 = mFileIni.Select_GetIniString(ref menuSoft6, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
                    }
                    catch (Exception ex3)
                    {
                        ProjectData.SetProjectError(ex3);
                        Exception ex4 = ex3;
                        ProjectData.ClearProjectError();
                    }
                }
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            InitProjetInstallShield();
            base.Opacity = MyProject.Forms.FormBox.nvOpacity;
            TextPatchInformation.Text = "";
            ToolStripStatusLabel1.Text = "";
            System.Windows.Forms.Application.DoEvents();
            ListPatch2Combo();
            if (Operators.CompareString(Application, "", TextCompare: false) != 0)
            {
                if (!Directory.Exists(Application))
                {
                    Directory.CreateDirectory(Application);
                }
            }
            else
            {
                ButtonGetVersion.Enabled = false;
            }
            CheckFolderEmpty();
            ButtonCreating.Enabled = Operators.CompareString(MyProject.Forms.FormBox.svAppBuild, null, TextCompare: false) != 0;
            CheckBoxExe.Checked = true;
            CheckBoxZip.Checked = true;
            CheckBoxExe.Visible = MyProject.Forms.FormBox.bAdvancedMenu;
            CheckBoxZip.Visible = MyProject.Forms.FormBox.bAdvancedMenu;
        }

        public void ListPatch2Combo()
        {
            ComboBoxPatchName.Items.Clear();
            foreach (string key in MyProject.Forms.FormBox.DicExes.Keys)
            {
                ComboBoxPatchName.Items.Add(key);
            }
        }

        private double ConvDate(string svDate)
        {
            return Conversion.Val(Strings.Mid(svDate, 7, 4) + Strings.Mid(svDate, 4, 2) + Strings.Mid(svDate, 1, 2) + Strings.Mid(svDate, 12, 2) + Strings.Mid(svDate, 15, 2) + Strings.Mid(svDate, 18, 2));
        }

        private void ButtonCreating_Click(object sender, EventArgs e)
        {
            CreateUpdate_Enabled(Enable: false);
            ComboBoxPatchName.Text.Trim();

            MessageBox.Show("Début ButtonCreating_Click");

            if ((Operators.CompareString(TextPatchInformation.Text, "", TextCompare: false) != 0) & (Operators.CompareString(ComboBoxPatchName.Text, "", TextCompare: false) != 0))
            {
                MessageBox.Show("1");

                string text = MyProject.Forms.FormBox.mPathExes + ComboBoxPatchName.Text.Replace(MyProject.Forms.FormBox.mExtFileExes, "").Replace(MyProject.Forms.FormBox.mExtFileZips, "");
                if (!File.Exists(text + MyProject.Forms.FormBox.mExtFileExes) & !File.Exists(text + MyProject.Forms.FormBox.mExtFileZips))
                {
                    MessageBox.Show("2");

                    if (Operators.CompareString(Application, "", TextCompare: false) != 0)
                    {
                        MessageBox.Show("3");

                        string[] files = Directory.GetFiles(Application, "*.*", SearchOption.AllDirectories);
                        files = files.Select((string w) => mFunction.ParsePath(w, 5)).ToArray();
                        if (Operators.CompareString(ExcludedFiles[0], "", TextCompare: false) != 0)
                        {
                            MessageBox.Show("4");

                            string text2 = null;
                            string[] excludedFiles = ExcludedFiles;
                            foreach (string text3 in excludedFiles)
                            {
                                if (files.Contains(text3))
                                {
                                    text2 = text2 + "\r\n- " + text3;
                                }
                            }

                            MessageBox.Show("Text2 : " + text2);

                            if (Operators.CompareString(text2, "", TextCompare: false) != 0 && Interaction.MsgBox("Warning: Exclude files presents" + text2, MsgBoxStyle.OkCancel | MsgBoxStyle.Critical | MsgBoxStyle.DefaultButton2, "Exclude files") == MsgBoxResult.Cancel)
                            {
                                MessageBox.Show("CreateUpdate_Enabled");

                                CreateUpdate_Enabled();
                                return;
                            }
                        }
                    }
                    string path = mFunction.ParsePath(FileTxtInfoPatch, 6);
                    if (!Directory.Exists(path))
                    {
                        Directory.CreateDirectory(path);
                    }
                    if (File.Exists(FileTxtInfoPatch))
                    {
                        File.Delete(FileTxtInfoPatch);
                    }
                    string contents = Strings.Replace(TextPatchInformation.Text, ",", ";");
                    File.WriteAllText(FileTxtInfoPatch, contents, Encoding.Default);
                    File.WriteAllText(text + MyProject.Forms.FormBox.mExtFileInfo, contents, Encoding.Default);

                    MessageBox.Show("Avant if GenerateZip");
                    if (CheckBoxZip.Checked & (Operators.CompareString(BuildZipLocation, "", TextCompare: false) != 0))
                    {
                        MessageBox.Show("Avant GenerateZip : " + BuildZipLocation + "_____" + text + MyProject.Forms.FormBox.mExtFileZips);
                        GenerateZip(BuildZipLocation, text + MyProject.Forms.FormBox.mExtFileZips);
                    }

                    MessageBox.Show("CheckBoxExe.Checked");
                    if (CheckBoxExe.Checked)
                    {
                        MessageBox.Show("Avant CreatePatch : " + text + MyProject.Forms.FormBox.mExtFileExes);
                        CreatePatch(text + MyProject.Forms.FormBox.mExtFileExes);
                    }
                    MyProject.Forms.FormBox.CreateListExes();
                    ListPatch2Combo();
                }
                else
                {
                    Interaction.MsgBox("The patch already exist :\r\n\"" + text + "\"", MsgBoxStyle.Exclamation);
                }
            }
            else
            {
                Interaction.MsgBox("the fields \"Package name\" or/and \"Infomation Package\" are empty", MsgBoxStyle.Critical, "Empty fields");
            }
            CreateUpdate_Enabled();
        }

        private void ButtonExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void ComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (Operators.CompareString(TextPatchInformation.Text, null, TextCompare: false) == 0)
            {
                string path = MyProject.Forms.FormBox.mPathExes + mFunction.ParsePath(ComboBoxPatchName.Text, 5) + MyProject.Forms.FormBox.mExtFileInfo;
                if (File.Exists(path))
                {
                    TextPatchInformation.Text = File.ReadAllText(path, Encoding.Default);
                }
            }
        }

        private void ButtonGetData_Click(object sender, EventArgs e)
        {
            CreateUpdate_Enabled(Enable: false);
            ref string menuSoft = ref MyProject.Forms.FormBox.MenuSoft;
            string MotCle = "DataRecovery";
            if (Operators.CompareString(mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim(), "P", TextCompare: false) == 0)
            {
                MyProject.Forms.DataRecoveryP.ShowDialog();
                TextPatchInformation.Text = MyProject.Forms.DataRecoveryP.NameOfWave;
            }
            else
            {
                ref string menuSoft2 = ref MyProject.Forms.FormBox.MenuSoft;
                MotCle = "DataRecovery";
                if (Operators.CompareString(mFileIni.Select_GetIniString(ref menuSoft2, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim(), "I", TextCompare: false) == 0)
                {
                    MyProject.Forms.DataRecovery.ShowDialog();
                    TextPatchInformation.Text = MyProject.Forms.DataRecovery.NameOfWave;
                }
                else
                {
                    ref string menuSoft3 = ref MyProject.Forms.FormBox.MenuSoft;
                    MotCle = "DataRecovery";
                    if (Operators.CompareString(mFileIni.Select_GetIniString(ref menuSoft3, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim(), "C", TextCompare: false) == 0)
                    {
                        ref string menuSoft4 = ref MyProject.Forms.FormBox.MenuSoft;
                        MotCle = "Source";
                        string text = mFileIni.Select_GetIniString(ref menuSoft4, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
                        if (!Directory.Exists(text))
                        {
                            text = Path.Combine(Environment.GetEnvironmentVariable("USERPROFILE"), "Downloads");
                        }
                        OpenFileDialog1.InitialDirectory = text;
                        OpenFileDialog1.Filter = "zip files (*.zip)|*.zip|All files (*.*)|*.*";
                        OpenFileDialog1.FilterIndex = 1;
                        OpenFileDialog1.RestoreDirectory = true;
                        OpenFileDialog1.Multiselect = false;
                        OpenFileDialog1.FileName = "";
                        OpenFileDialog1.ShowDialog();
                        if (File.Exists(OpenFileDialog1.FileName))
                        {
                            string text2 = Path.GetTempPath() + MyProject.Forms.FormBox.MenuSoft;
                            if (Directory.Exists(text2))
                            {
                                Directory.Delete(text2, recursive: true);
                                if (!Directory.Exists(text2))
                                {
                                    Directory.CreateDirectory(text2);
                                }
                            }
                            ToolStripStatusLabel1.Text = "Unzip in progress...";
                            ToolStripProgressBar1.Style = ProgressBarStyle.Marquee;
                            ToolStripProgressBar1.MarqueeAnimationSpeed = 20;
                            System.Windows.Forms.Application.DoEvents();
                            MyProject.Forms.DataRecovery.ExtractZip(OpenFileDialog1.FileName, text2);
                            ToolStripStatusLabel1.Text = "Copy in progress...";
                            System.Windows.Forms.Application.DoEvents();
                            ref string menuSoft5 = ref MyProject.Forms.FormBox.MenuSoft;
                            MotCle = "Survey";
                            string destinationFolder = mFileIni.Select_GetIniString(ref menuSoft5, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
                            if (CopyLstFile2Dest("study.lst", text2, destinationFolder))
                            {
                                ref string menuSoft6 = ref MyProject.Forms.FormBox.MenuSoft;
                                MotCle = "Vendor";
                                destinationFolder = mFileIni.Select_GetIniString(ref menuSoft6, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
                                if (!CopyLstFile2Dest("vendorU.lst", text2, destinationFolder, bVendorFile: true))
                                {
                                    Interaction.MsgBox("File 'vendorU.lst' not found.", MsgBoxStyle.Exclamation);
                                }
                                string[] files = Directory.GetFiles(text2, "study.inf", SearchOption.AllDirectories);
                                if (files.Length > 0)
                                {
                                    string[] array = File.ReadAllLines(files[0]);
                                    TextPatchInformation.Text = array[1];
                                    ComboBoxPatchName.Text = array[0];
                                }
                                else
                                {
                                    Interaction.MsgBox("File 'study.inf' not found.", MsgBoxStyle.Exclamation);
                                }
                            }
                            else
                            {
                                Interaction.MsgBox("File 'study.lst' not found.", MsgBoxStyle.Critical);
                            }
                            Directory.Delete(text2, recursive: true);
                            ToolStripStatusLabel1.Text = "Completed.";
                            ToolStripProgressBar1.Style = ProgressBarStyle.Continuous;
                            System.Windows.Forms.Application.DoEvents();
                        }
                    }
                }
            }
            CreateUpdate_Enabled();
        }

        private bool CopyLstFile2Dest(string szfile, string SourceFolder, string DestinationFolder, bool bVendorFile = false)
        {
            checked
            {
                try
                {
                    string[] files = Directory.GetFiles(SourceFolder, szfile, SearchOption.AllDirectories);
                    if (files.Length == 0)
                    {
                        return false;
                    }
                    string[] array = files;
                    foreach (string obj in array)
                    {
                        string text = mFunction.ParsePath(obj, 6);
                        string[] array2 = File.ReadAllLines(obj, Encoding.Default);
                        string[] array3 = array2;
                        for (int j = 0; j < array3.Length; j++)
                        {
                            string text2 = array3[j];
                            text2 = text2.Replace("/", "\\");
                            string text3 = mFunction.ParsePath(text2, 4);
                            string text4 = text + text3;
                            if (!File.Exists(text4))
                            {
                                text4 = Directory.GetFiles(text, text3, SearchOption.AllDirectories)[0];
                            }
                            string text5 = "";
                            if (bVendorFile)
                            {
                                switch (mFunction.ParsePath(text3, 3))
                                {
                                    case ".DFX":
                                        {
                                            string text7 = ReadSurveyCode(text4);
                                            if (Operators.CompareString(text7, "", TextCompare: false) != 0)
                                            {
                                                text5 = text7 + "\\";
                                            }
                                            break;
                                        }
                                    case ".DEF":
                                        {
                                            string text8 = ReadFirstLine(text4);
                                            if (Operators.CompareString(text8, "", TextCompare: false) != 0)
                                            {
                                                text5 = text8 + "\\";
                                            }
                                            break;
                                        }
                                    case ".DFU":
                                        {
                                            string text6 = ReadFirstLine(text4);
                                            if (Operators.CompareString(text6, "", TextCompare: false) != 0)
                                            {
                                                text5 = text6 + "\\";
                                            }
                                            break;
                                        }
                                }
                            }
                            string text9 = DestinationFolder + text5 + text2;
                            string path = mFunction.ParsePath(text9, 6);
                            if (!Directory.Exists(path))
                            {
                                Directory.CreateDirectory(path);
                            }
                            if (File.Exists(text9))
                            {
                                File.Delete(text9);
                            }
                            File.Copy(text4, text9);
                        }
                        string[] array4 = array2;
                        for (int k = 0; k < array4.Length; k++)
                        {
                            string text10 = mFunction.ParsePath(array4[k].Replace("/", "\\"), 4);
                            File.Delete(text + text10);
                        }
                    }
                    return true;
                }
                catch (Exception ex)
                {
                    ProjectData.SetProjectError(ex);
                    Exception ex2 = ex;
                    Interaction.MsgBox(ex2.Message, MsgBoxStyle.Critical);
                    bool result = false;
                    ProjectData.ClearProjectError();
                    return result;
                }
            }
        }

        private string ReadFirstLine(string szFile)
        {
            return File.ReadAllLines(szFile)[0].Replace("\0", "");
        }

        private string ReadSurveyCode(string szFile)
        {
            string szText = File.ReadAllText(szFile);
            return ExtractSurveyCode(szText);
        }

        private string ExtractSurveyCode(string szText)
        {
            checked
            {
                int num = Strings.InStr(Strings.InStr(szText, "SurveyCode", CompareMethod.Text), szText, "\"", CompareMethod.Text) + 1;
                int num2 = Strings.InStr(num, szText, "\"", CompareMethod.Text);
                return Strings.Mid(szText, num, num2 - num);
            }
        }

        private void PatchRadioKey(string KeyPath, string Pigedata)
        {
            ref string menuSoft = ref MyProject.Forms.FormBox.MenuSoft;
            string MotCle = "Key1";
            string text = mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft2 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "KeyFile";
            string text2 = mFileIni.Select_GetIniString(ref menuSoft2, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            int num = 1;
            string path = mFunction.ParsePath(FileTxtInfoPatch, 6);
            if (!Directory.Exists(path))
            {
                Directory.CreateDirectory(path);
            }
            if (File.Exists(FileTxtInfoPatch))
            {
                File.Delete(FileTxtInfoPatch);
            }
            string contents = Strings.Replace(TextPatchInformation.Text, ",", ";");
            File.WriteAllText(FileTxtInfoPatch, contents, Encoding.Default);
            while (Operators.CompareString(text, "", TextCompare: false) != 0)
            {
                string[] array = Strings.Split(text, ",");
                string text3 = MyProject.Forms.FormBox.mPathExes + ComboBoxPatchName.Text + array[1];
                if (File.Exists(text3 + MyProject.Forms.FormBox.mExtFileExes))
                {
                    Interaction.MsgBox("The patch already exist :\r\n\"" + text3 + MyProject.Forms.FormBox.mExtFileExes + "\"", MsgBoxStyle.Exclamation);
                    return;
                }
                if (File.Exists(text3 + MyProject.Forms.FormBox.mExtFileZips))
                {
                    Interaction.MsgBox("The patch already exist :\r\n\"" + text3 + MyProject.Forms.FormBox.mExtFileZips + "\"", MsgBoxStyle.Exclamation);
                    return;
                }
                string text4 = KeyPath + array[0];
                if (!Directory.Exists(Pigedata))
                {
                    Directory.CreateDirectory(Pigedata);
                }
                try
                {
                    File.Delete(Pigedata + text2);
                }
                catch (Exception ex)
                {
                    ProjectData.SetProjectError(ex);
                    Exception ex2 = ex;
                    Interaction.MsgBox("Unable to delete: " + Pigedata + text2 + "\r\nError: " + ex2.ToString(), MsgBoxStyle.Critical);
                    ProjectData.ClearProjectError();
                    return;
                }
                if (File.Exists(text4))
                {
                    try
                    {
                        File.Copy(text4, Pigedata + text2);
                    }
                    catch (Exception ex3)
                    {
                        ProjectData.SetProjectError(ex3);
                        Exception ex4 = ex3;
                        Interaction.MsgBox("Unable to copy: " + array[0] + "\r\nError: " + ex4.ToString(), MsgBoxStyle.Critical);
                        ProjectData.ClearProjectError();
                        return;
                    }
                }
                File.WriteAllText(text3 + MyProject.Forms.FormBox.mExtFileInfo, contents, Encoding.Default);
                if (CheckBoxZip.Checked & (Operators.CompareString(BuildZipLocation, "", TextCompare: false) != 0))
                {
                    GenerateZip(BuildZipLocation, text3 + MyProject.Forms.FormBox.mExtFileZips);
                }
                if (CheckBoxExe.Checked)
                {
                    CreatePatch(text3 + MyProject.Forms.FormBox.mExtFileExes);
                }
                num = checked(num + 1);
                ref string menuSoft3 = ref MyProject.Forms.FormBox.MenuSoft;
                MotCle = "Key" + Strings.Trim(Conversions.ToString(num));
                text = mFileIni.Select_GetIniString(ref menuSoft3, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            }
            MyProject.Forms.FormBox.CreateListExes();
            ComboBoxPatchName.Text = "";
            ListPatch2Combo();
        }

        private void PatchRadioGroupKey(string KeyPath, string Pigedata)
        {
            ref string menuSoft = ref MyProject.Forms.FormBox.MenuSoft;
            string MotCle = "KeyGroup0";
            string text = mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            if (Operators.CompareString(text, null, TextCompare: false) == 0)
            {
                return;
            }
            string text2 = MyProject.Forms.FormBox.mPathExes + ComboBoxPatchName.Text + text;
            if (File.Exists(text2))
            {
                Interaction.MsgBox("The patch already exist :\r\n\"" + text2 + "\"", MsgBoxStyle.Exclamation);
                return;
            }
            ref string menuSoft2 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "KeyFile";
            string text3 = mFileIni.Select_GetIniString(ref menuSoft2, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            if (Operators.CompareString(text3, null, TextCompare: false) == 0)
            {
                return;
            }
            string text4 = mFunction.ParsePath(text3, 5);
            string text5 = mFunction.ParsePath(text3, 3);
            File.Delete(Pigedata + text4 + "*." + text5);
            ref string menuSoft3 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "KeyGroup1";
            text = mFileIni.Select_GetIniString(ref menuSoft3, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            if (Operators.CompareString(text, null, TextCompare: false) == 0)
            {
                return;
            }
            int num = 1;
            while (Operators.CompareString(text, "", TextCompare: false) != 0)
            {
                string[] array = Strings.Split(text, ",");
                string text6 = KeyPath + array[0];
                if (!Directory.Exists(Pigedata))
                {
                    Directory.CreateDirectory(Pigedata);
                }
                if (File.Exists(text6))
                {
                    try
                    {
                        File.Copy(text6, Pigedata + text4 + array[1] + text5);
                    }
                    catch (Exception ex)
                    {
                        ProjectData.SetProjectError(ex);
                        Exception ex2 = ex;
                        Interaction.MsgBox("Unable to copy: " + FileTxtInfoPatch + "\r\nError: " + ex2.ToString(), MsgBoxStyle.Critical);
                        ProjectData.ClearProjectError();
                        return;
                    }
                }
                num = checked(num + 1);
                ref string menuSoft4 = ref MyProject.Forms.FormBox.MenuSoft;
                MotCle = "KeyGroup" + Strings.Trim(Conversions.ToString(num));
                text = mFileIni.Select_GetIniString(ref menuSoft4, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            }
            CreatePatch(MyProject.Forms.FormBox.mPathExes + ComboBoxPatchName.Text + text);
            MyProject.Forms.FormBox.CreateListExes();
            ComboBoxPatchName.Text = "";
            ListPatch2Combo();
        }

        private void CreatePatch(string Package)
        {
            MessageBox.Show("Début CreatePatch");

            string svCommandLine = "-s -p \"" + ProjectFile + "\" -r \"" + MediaName + "\" -b \"" + BuildLocation + "\"";
            ToolStripProgressBar1.Style = ProgressBarStyle.Marquee;
            ToolStripStatusLabel1.Text = "Creating in Progress...";
            string svAppBuild = MyProject.Forms.FormBox.svAppBuild;

            //TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Info, "svAppBuild :" + svAppBuild);
            MessageBox.Show("svAppBuild :" + svAppBuild);

            ToolStripStatusLabel1.Text = "svAppBuild :" + svAppBuild;

            ProcessWindowStyle Style = ProcessWindowStyle.Hidden;
            byte WaitInfinity = 1;
            
            //TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Info, "Avant LaunchAppAndWait");
            MessageBox.Show("Avant LaunchAppAndWait");
            ToolStripStatusLabel1.Text = "Avant LaunchAppAndWait";

            mLaunchAppAndWait.LaunchAppAndWait(svAppBuild, svCommandLine, Style, WaitInfinity);
            //TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Info, "Après LaunchAppAndWait");
            MessageBox.Show("Après LaunchAppAndWait");
            ToolStripStatusLabel1.Text = "Après LaunchAppAndWait";

            ToolStripProgressBar1.Style = ProgressBarStyle.Continuous;

            //TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Info, "PackageLocation : " + PackageLocation);
            MessageBox.Show("PackageLocation : " + PackageLocation);
            ToolStripStatusLabel1.Text = "PackageLocation : " + PackageLocation;

            if (!File.Exists(PackageLocation))
            {
                //TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Error, "PackageLocation n'est pas accessible.");
                MessageBox.Show("PackageLocation n'est pas accessible.");
                ToolStripStatusLabel1.Text = "PackageLocation n'est pas accessible.";

                return;
            }
            try
            {
                ToolStripProgressBar1.Maximum = 100;
                ToolStripStatusLabel1.Text = "CopyFile Progress...";
                if (mCopyFile.CopyFileWithProgress(ref PackageLocation, ref Package, 1))
                {
                    File.Delete(PackageLocation);
                }
                else
                {
                    Interaction.MsgBox("Unable to move: " + PackageLocation + "\r\nError: CopyFileWithProgress", MsgBoxStyle.Critical);
                }
                ToolStripProgressBar1.Value = 0;
                ToolStripStatusLabel1.Text = "";
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                if (Interaction.MsgBox("Unable to move: " + PackageLocation + "\r\nError: " + ex2.ToString() + "\r\n\r\nWould you like to retry?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo, "CopyFileWithProgress") == MsgBoxResult.Yes)
                {
                    if (File.Exists(PackageLocation))
                    {
                        if (File.Exists(Package))
                        {
                            try
                            {
                                File.Delete(Package);
                            }
                            catch (Exception ex3)
                            {
                                ProjectData.SetProjectError(ex3);
                                Exception ex4 = ex3;
                                Interaction.MsgBox("Unable to delete: " + Package + "\r\nError: " + ex4.ToString(), MsgBoxStyle.Critical, "Delete file");
                                ProjectData.ClearProjectError();
                            }
                        }
                        try
                        {
                            File.Move(PackageLocation, Package);
                        }
                        catch (Exception ex5)
                        {
                            ProjectData.SetProjectError(ex5);
                            Exception ex6 = ex5;
                            Interaction.MsgBox("Unable to move: " + PackageLocation + "\r\nError: " + ex6.ToString(), MsgBoxStyle.Critical, "Move file");
                            ProjectData.ClearProjectError();
                        }
                    }
                    else
                    {
                        Interaction.MsgBox("Unable to move: " + PackageLocation + "\r\nError: Not found", MsgBoxStyle.Critical);
                    }
                }
                ProjectData.ClearProjectError();
            }
        }

        private void ButtonGetVersion_Click(object sender, EventArgs e)
        {
            CreateUpdate_Enabled(Enable: false);
            ToolStripProgressBar1.Style = ProgressBarStyle.Marquee;
            try
            {
                MyProject.Forms.GetElements.ComboBoxVersion.Items.Clear();
                MyProject.Forms.GetElements.ComboBoxVersion.Text = "";
                MyProject.Forms.GetElements.Text = "Get Version";
                MyProject.Forms.GetElements.ChooseLabel.Text = "Choose Version:";
                string text = MyProject.Forms.FormBox.CountryDic[MyProject.Forms.FormBox.MenuPays].Replace("Acces_", "").Replace("\\", "");
                if (Strings.Len(text) == 2)
                {
                    if (Directory.Exists(VersionFolder.Replace("##", text)))
                    {
                        VersionFolder = VersionFolder.Replace("##", text);
                    }
                    else
                    {
                        VersionFolder = VersionFolder.Replace("##", "FR");
                    }
                }
                else
                {
                    VersionFolder = VersionFolder.Replace("##", "FR");
                }
                if (Operators.CompareString(VersionFolder, "", TextCompare: false) != 0)
                {
                    if (Operators.CompareString(FilterDirectory, "", TextCompare: false) != 0)
                    {
                        string[] directories = Directory.GetDirectories(VersionFolder, FilterDirectory);
                        directories = (from w in directories
                                       select (w) into w
                                       orderby Directory.GetCreationTime(w) descending
                                       select w).ToArray();
                        directories = directories.Select((string w) => Strings.Replace(w, VersionFolder, "")).ToArray();
                        MyProject.Forms.GetElements.ComboBoxVersion.Items.AddRange(directories);
                    }
                    else if (Operators.CompareString(FilterVersion, "", TextCompare: false) != 0)
                    {
                        string[] files = Directory.GetFiles(VersionFolder, FilterVersion);
                        files = (from w in files
                                 select (w) into w
                                 orderby File.GetCreationTime(w) descending
                                 select w).ToArray();
                        files = files.Select((string w) => Strings.Replace(w, VersionFolder, "")).ToArray();
                        MyProject.Forms.GetElements.ComboBoxVersion.Items.AddRange(files);
                    }
                }
                if (MyProject.Forms.GetElements.ComboBoxVersion.Items.Count > 0)
                {
                    if (MyProject.Forms.GetElements.ShowDialog() == DialogResult.OK)
                    {                        
                        //PathVersion pathVersion = new PathVersion();
                        if (Directory.GetFiles(Application, "*.*", SearchOption.AllDirectories).Count() > 0 && Interaction.MsgBox("Application Directory is not empty.\r\n\r\nDo you really want to erase?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo, "Application directory") == MsgBoxResult.Yes)
                        {
                            Directory.Delete(Application, recursive: true);
                            Directory.CreateDirectory(Application);
                        }

                        string pathVersion = VersionFolder + MyProject.Forms.GetElements.ComboBoxVersion.Text.Trim();
                        if (File.Exists(pathVersion))
                        {
                            MyProject.Forms.DataRecovery.ExtractZip(pathVersion, Application);
                            goto IL_0488;
                        }
                        if (Directory.Exists(pathVersion))
                        {
                            pathVersion += "\\";
                            string[] array = (from w in Directory.GetFiles(pathVersion, "*.*", SearchOption.AllDirectories)
                                              select Strings.Replace(w, pathVersion, "")).ToArray();
                            foreach (string text2 in array)
                            {
                                string path = mFunction.ParsePath(Application + text2, 6);
                                if (!Directory.Exists(path))
                                {
                                    Directory.CreateDirectory(path);
                                }
                                string value = mFunction.ParsePath(Application + text2, 5);
                                if (!ExcludedFiles.Contains(value))
                                {
                                    if (File.Exists(Application + text2))
                                    {
                                        File.Delete(Application + text2);
                                    }
                                    File.Copy(pathVersion + text2, Application + text2);
                                }
                            }
                            goto IL_0488;
                        }
                    }
                    else
                    {
                        TextPatchInformation.Text = "";
                        MyProject.Forms.GetElements.ComboBoxVersion.Text = "";
                    }
                }
                goto end_IL_0013;
            IL_0488:
                if (!Directory.Exists(Application + "\\FRET"))
                {
                    Directory.CreateDirectory(Application + "\\FRET");
                }
                if (File.Exists(Application + "\\FRET\\Version.txt"))
                {
                    File.Delete(Application + "\\FRET\\Version.txt");
                }
                int num = Strings.InStr(FilterDirectory, "?", CompareMethod.Text);
                string text3 = null;
                if (num > 0)
                {
                    text3 = Strings.Mid(MyProject.Forms.GetElements.ComboBoxVersion.Text, num, checked(Strings.Len(MyProject.Forms.GetElements.ComboBoxVersion.Text) - num + 1));
                    text3 = Strings.Replace(text3, ".exe", "", 1, 1, CompareMethod.Text);
                    File.WriteAllText(Application + "\\FRET\\Version.txt", text3);
                    TextPatchInformation.Text = "Version: " + text3;
                }
            end_IL_0013:;
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                Interaction.MsgBox(ex2.Message, MsgBoxStyle.Critical);
                ProjectData.ClearProjectError();
            }
            ToolStripProgressBar1.Style = ProgressBarStyle.Continuous;
            CreateUpdate_Enabled();
        }

        private void CreateUpdate_Enabled(bool Enable = true)
        {
            if (!Enable)
            {
                bMem_ButtonCreating = ButtonCreating.Enabled;
                bMem_ButtonGetData = ButtonGetData.Enabled;
                bMem_ButtonPigeAuto = ButtonPigeAuto.Enabled;
                bMem_ButtonGetTarifs = ButtonGetTarifs.Enabled;
                bMem_ButtonGetVersions = ButtonGetVersion.Enabled;
                bMem_ComboBox1 = ComboBoxPatchName.Enabled;
                bMem_TextBox2 = TextPatchInformation.Enabled;
                ButtonCreating.Enabled = false;
                ButtonGetData.Enabled = false;
                ButtonPigeAuto.Enabled = false;
                ButtonGetTarifs.Enabled = false;
                ButtonGetVersion.Enabled = false;
                ComboBoxPatchName.Enabled = false;
                TextPatchInformation.Enabled = false;
            }
            else
            {
                ButtonCreating.Enabled = bMem_ButtonCreating;
                ButtonGetData.Enabled = bMem_ButtonGetData;
                ButtonPigeAuto.Enabled = bMem_ButtonPigeAuto;
                ButtonGetTarifs.Enabled = bMem_ButtonGetTarifs;
                ButtonGetVersion.Enabled = bMem_ButtonGetVersions;
                ComboBoxPatchName.Enabled = bMem_ComboBox1;
                TextPatchInformation.Enabled = bMem_TextBox2;
            }
        }

        private void ButtonPigeAuto_Click(object sender, EventArgs e)
        {
            CreateUpdate_Enabled(Enable: false);
            ref string menuSoft = ref MyProject.Forms.FormBox.MenuSoft;
            string MotCle = "PigeName";
            string text = mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            if (Operators.CompareString(ComboBoxPatchName.Text.Trim(), "", TextCompare: false) == 0)
            {
                Interaction.MsgBox("Warning: Package Name is empty", MsgBoxStyle.Critical, "Create patch");
                CreateUpdate_Enabled();
                return;
            }
            if (Operators.CompareString(TextPatchInformation.Text, "", TextCompare: false) == 0)
            {
                Interaction.MsgBox("Warning: Package Information is empty", MsgBoxStyle.Critical, "Create patch");
                CreateUpdate_Enabled();
                return;
            }
            if (Interaction.MsgBox("Are you sure you want to automatically create patches (\"" + text + "\")?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo | MsgBoxStyle.DefaultButton2, "Create patch") == MsgBoxResult.No)
            {
                CreateUpdate_Enabled();
                return;
            }
            MyProject.Forms.GetElements.ComboBoxVersion.Items.Clear();
            MyProject.Forms.GetElements.ComboBoxVersion.Text = "";
            MyProject.Forms.GetElements.Text = "Get data file patch";
            MyProject.Forms.GetElements.ChooseLabel.Text = "Please select the data file patch";
            MyProject.Forms.GetElements.ButtonGet.Text = "Get";
            Icon icon = Icon.FromHandle(Resources.ImportData.GetHicon());
            MyProject.Forms.GetElements.Icon = icon;
            ref string menuSoft2 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "KeyPath";
            string keyPath = mFileIni.Select_GetIniString(ref menuSoft2, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft3 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "Pigedata";
            string text2 = mFileIni.Select_GetIniString(ref menuSoft3, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft4 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "DataPath";
            string text3 = mFileIni.Select_GetIniString(ref menuSoft4, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft5 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "DataFind";
            string searchPattern = mFileIni.Select_GetIniString(ref menuSoft5, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft6 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "NamePige";
            mFileIni.Select_GetIniString(ref menuSoft6, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            if (Operators.CompareString(text3, "", TextCompare: false) != 0)
            {
                string[] files = Directory.GetFiles(text3, searchPattern);
                files = (from w in files
                         select (w) into w
                         orderby File.GetCreationTime(w) descending
                         select w).ToArray();
                files = files.Select((string w) => Strings.Replace(w, text3, "")).ToArray();
                MyProject.Forms.GetElements.ComboBoxVersion.Items.AddRange(files);
            }
            if (MyProject.Forms.GetElements.ComboBoxVersion.Items.Count > 0)
            {
                MyProject.Forms.GetElements.ComboBoxVersion.Text = Conversions.ToString(MyProject.Forms.GetElements.ComboBoxVersion.Items[0]);
            }
            switch (MyProject.Forms.GetElements.ShowDialog())
            {
                case DialogResult.OK:
                    {
                        string text4 = MyProject.Forms.GetElements.ComboBoxVersion.Text;
                        if (Operators.CompareString(text4, "", TextCompare: false) != 0)
                        {
                            if (File.Exists(text3 + text4))
                            {
                                File.Copy(text3 + text4, text2 + text4, overwrite: true);
                            }
                            if (File.Exists(text3 + "InfosRad.txt"))
                            {
                                File.Copy(text3 + "InfosRad.txt", text2 + "InfosRad.txt", overwrite: true);
                            }
                        }
                        PatchRadioKey(keyPath, text2);
                        Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal, "Create patch");
                        break;
                    }
                case DialogResult.Ignore:
                    if (File.Exists(text3 + "InfosRad.txt"))
                    {
                        File.Copy(text3 + "InfosRad.txt", text2 + "InfosRad.txt", overwrite: true);
                    }
                    PatchRadioKey(keyPath, text2);
                    Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal, "Create patch");
                    break;
            }
            CreateUpdate_Enabled();
        }

        private void ButtonGetTarifs_Click(object sender, EventArgs e)
        {
            CreateUpdate_Enabled(Enable: false);
            try
            {
                OpenFileDialogVersion.Filter = "EXE Files (*.exe)|*.exe|ZIP files (*.zip)|*.zip";
                if (Operators.CompareString(TarifsFolder, null, TextCompare: false) != 0)
                {
                    OpenFileDialogVersion.InitialDirectory = TarifsFolder;
                }
                else
                {
                    OpenFileDialogVersion.InitialDirectory = VersionFolder;
                }
                OpenFileDialogVersion.FileName = "";
                if (OpenFileDialogVersion.ShowDialog() == DialogResult.OK)
                {
                    MyProject.Forms.DataRecovery.ExtractZip(OpenFileDialogVersion.FileName, Tarifs, "ZONE");
                }
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                Interaction.MsgBox(ex2.Message, MsgBoxStyle.Critical);
                ProjectData.ClearProjectError();
            }
            CreateUpdate_Enabled();
        }

        private void HandleKeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                Close();
            }
        }

        public void GenerateZip(string inputPath, string targetLocation)
        {
            string[] files = Directory.GetFiles(inputPath, "*.*", SearchOption.AllDirectories);
            ToolStripProgressBar1.Value = 0;
            ToolStripStatusLabel1.Text = "";
            System.Windows.Forms.Application.DoEvents();
            List<string[]> list = new List<string[]>();
            inputPath = Directory.GetParent(inputPath).FullName;
            checked
            {
                int startIndex = inputPath.Length + 1;
                int num = 0;
                string[] array = files;
                foreach (string text in array)
                {
                    list.Add(new string[2]
                    {
                    text,
                    text.Substring(startIndex)
                    });
                    num += (int)Math.Round((double)FileSystem.FileLen(text) / 1024.0);
                }
                ToolStripProgressBar1.Maximum = num;
                if (File.Exists(targetLocation))
                {
                    File.Delete(targetLocation);
                }
                FileStream fileStream = new FileStream(targetLocation, FileMode.CreateNew);
                ZipOutputStream zipOutputStream = new ZipOutputStream(fileStream)
                {
                    Password = "JFC123"
                };
                foreach (string[] item in list)
                {
                    string text2 = item[1];
                    string text3 = item[0];
                    if (text2.Contains("\\") & (Strings.InStr(text2, "ICON\\", CompareMethod.Text) != 1))
                    {
                        int num2 = Strings.InStrRev(text3, "\\");
                        ToolStripStatusLabel1.Text = "Zip: " + Strings.Mid(text3, num2 + 1, text3.Length - num2);
                        System.Windows.Forms.Application.DoEvents();
                        ZipEntry zipEntry = new ZipEntry(ZipEntry.CleanName(text2));
                        File.GetCreationTime(text3);
                        File.GetLastAccessTime(text3);
                        DateTime dateTime = (zipEntry.DateTime = File.GetLastWriteTime(text3));
                        zipEntry.IsUnicodeText = true;
                        zipEntry.AESKeySize = 256;
                        zipOutputStream.PutNextEntry(zipEntry);
                        FileStream fileStream2 = File.OpenRead(text3);
                        int num3 = 65536;
                        byte[] array2 = new byte[num3 - 1 + 1];
                        int num4 = fileStream2.Read(array2, 0, num3);
                        while (num4 != 0)
                        {
                            zipOutputStream.Write(array2, 0, num4);
                            num4 = fileStream2.Read(array2, 0, num3);
                            ToolStripProgressBar1.Value += (int)Math.Round((double)num4 / 1024.0);
                            System.Windows.Forms.Application.DoEvents();
                        }
                        zipOutputStream.CloseEntry();
                        fileStream2.Close();
                    }
                    System.Windows.Forms.Application.DoEvents();
                }
                ToolStripProgressBar1.Value = 0;
                ToolStripStatusLabel1.Text = "";
                zipOutputStream.Finish();
                zipOutputStream.Close();
                fileStream.Close();
                System.Windows.Forms.Application.DoEvents();
            }
        }
    }
}
