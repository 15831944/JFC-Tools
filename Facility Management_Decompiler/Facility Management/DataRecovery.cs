using Facility_Management.My;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Windows.Forms;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;
using ICSharpCodeAU.SharpZipLib.Zip;

namespace Facility_Management
{

    public partial class DataRecovery : Form
    {
        private readonly string seprator;

        private string mPath;

        private string mDestination;

        public bool LaunchDirectClass;

        public string NameOfWave;

        public bool IsNotLockedTreeView;

        private bool ZipButtonVisible;

        private bool UnZipButtonVisible;

        private string ZipPassword;


        public DataRecovery()
        {
            base.Load += Form1_Load;
            seprator = " - ";
            IsNotLockedTreeView = true;
            ZipButtonVisible = false;
            UnZipButtonVisible = false;
            ZipPassword = "";
            InitializeComponent();
        }

        private void Init()
        {
            ref string menuSoft = ref MyProject.Forms.FormBox.MenuSoft;
            string MotCle = "Source";
            mPath = mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            ref string menuSoft2 = ref MyProject.Forms.FormBox.MenuSoft;
            MotCle = "Destination";
            mDestination = mFileIni.Select_GetIniString(ref menuSoft2, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
            MotCle = "Zip";
            string MotCle2 = "ButtonZipVisible";
            ZipButtonVisible = Operators.CompareString(mFileIni.Select_GetIniString(ref MotCle, ref MotCle2, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim(), "1", TextCompare: false) == 0;
            MotCle2 = "Zip";
            MotCle = "ButtonUnZipVisible";
            UnZipButtonVisible = Operators.CompareString(mFileIni.Select_GetIniString(ref MotCle2, ref MotCle, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim(), "1", TextCompare: false) == 0;
            MotCle = "Zip";
            MotCle2 = "ZipPassword";
            ZipPassword = mFileIni.Select_GetIniString(ref MotCle, ref MotCle2, ref MyProject.Forms.FormBox.FacilityManagementIni).Trim();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Init();
            base.Opacity = MyProject.Forms.FormBox.nvOpacity;
            Application.DoEvents();
            ReadSources();
            ButtonZip.Visible = ZipButtonVisible;
            ButtonUnZip.Visible = UnZipButtonVisible;
            ToolStripStatusLabel2.Text = "";
            ToolStripProgressBar2.Value = 0;
        }

        private void ReadSources()
        {
            ComboBox1.Items.Clear();
            ComboBox1.ResetText();
            TreeViewList.Nodes.Clear();
            TreeViewSelected.Nodes.Clear();
            string[] directories = Directory.GetDirectories(mPath);
            checked
            {
                int num = directories.Length - 1;
                for (int i = 0; i <= num; i++)
                {
                    string text = Strings.Replace(directories[i], mPath, "");
                    if (Operators.CompareString(ReadSourceName(mPath + text + "\\", text), "", TextCompare: false) != 0)
                    {
                        string item = text + seprator + ReadSourceName(mPath + text + "\\", text);
                        ComboBox1.Items.Add(item);
                    }
                }
                if (ComboBox1.Items.Count == 1)
                {
                    ComboBox1.SelectedIndex = 0;
                }
            }
        }

        private int AddNodes(TreeView node, string Libelle)
        {
            string key = (Libelle.Contains("2007") ? "2007" : (Libelle.Contains("2008") ? "2008" : (Libelle.Contains("2009") ? "2009" : (Libelle.Contains("2010") ? "2010" : (Libelle.Contains("2011") ? "2011" : (Libelle.Contains("2012") ? "2012" : (Libelle.Contains("2013") ? "2013" : (Libelle.Contains("2014") ? "2014" : (Libelle.Contains("2015") ? "2015" : (Libelle.Contains("2016") ? "2016" : (Libelle.Contains("2017") ? "2017" : (Libelle.Contains("2018") ? "2018" : (Libelle.Contains("2019") ? "2019" : ((!Libelle.Contains("2020")) ? "Year missing" : "2020"))))))))))))));
            if (!node.Nodes.ContainsKey(key))
            {
                node.Nodes.Add(key, key);
            }
            int num = node.Nodes.IndexOfKey(key);
            if (!node.Nodes[num].Nodes.ContainsKey(Libelle))
            {
                node.Nodes[num].Nodes.Add(Libelle, Libelle);
            }
            return num;
        }

        private void ComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            SelectedSources();
        }

        private void SelectedSources()
        {
            ButtonCreating.Enabled = false;
            ButtonZip.Enabled = false;
            ButtonUnZip.Enabled = false;
            string text = RecFirstName(Conversions.ToString(ComboBox1.SelectedItem));
            string text2 = mPath + text + "\\";
            string[] directories = Directory.GetDirectories(text2, "T?????.");
            checked
            {
                if (Directory.Exists(mDestination))
                {
                    string[] directories2 = Directory.GetDirectories(mDestination);
                    if (directories2.Length + unchecked(0 - (directories2.Contains(mDestination + text) ? 1 : 0)) > 0 && Interaction.MsgBox("Would you erase data from other countries", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo, "Clear Data") == MsgBoxResult.Yes)
                    {
                        string[] array = directories2;
                        foreach (string path in array)
                        {
                            if (!directories2.Contains(mDestination + text))
                            {
                                Directory.Delete(path, recursive: true);
                            }
                        }
                    }
                }
                else
                {
                    Directory.CreateDirectory(mDestination);
                }
                directories = (string[])Utils.CopyArray(directories, new string[directories.Length - 1 + 1]);
                TreeViewList.Nodes.Clear();
                TreeViewSelected.Nodes.Clear();
                TreeViewList.Sort();
                TreeViewSelected.Sort();
                int num = directories.Length - 1;
                for (int j = 0; j <= num; j++)
                {
                    string text3 = Strings.Replace(directories[j], text2, "");
                    string text4 = ReadVagueName(directories[j], text3);
                    if (Directory.Exists(mDestination + text + "\\" + text3))
                    {
                        AddNodes(TreeViewSelected, text3 + seprator + text4);
                    }
                    else
                    {
                        AddNodes(TreeViewList, text3 + seprator + text4);
                    }
                }
                ButtonCreating.Enabled = true;
                ButtonZip.Enabled = true;
                ButtonUnZip.Enabled = true;
            }
        }

        private string ReadSourceName(string svPath, string svFiles)
        {
            int i = 28;
            string text = "";
            string[] files = Directory.GetFiles(svPath, svFiles + "*.801", SearchOption.TopDirectoryOnly);
            if (files.Length > 0)
            {
                for (byte[] array = File.ReadAllBytes(files[0]); array[i] != 0; i = checked(i + 1))
                {
                    text += Conversions.ToString(Strings.Chr(array[i]));
                }
            }
            return text;
        }

        private string ReadVagueName(string svPath, string svFiles)
        {
            int i = 32;
            string text = "";
            for (byte[] array = File.ReadAllBytes(Directory.GetFiles(svPath, svFiles + "*.801", SearchOption.TopDirectoryOnly)[0]); array[i] != 0; i = checked(i + 1))
            {
                text += Conversions.ToString(Strings.Chr(array[i]));
            }
            return text;
        }

        private void ButtonExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void ButtonCreating_Click(object sender, EventArgs e)
        {
            ButtonCreating.Enabled = false;
            ButtonZip.Enabled = false;
            ButtonUnZip.Enabled = false;
            ToolStripProgressBar2.Maximum = CountFiles();
            ToolStripProgressBar2.Value = 0;
            NameOfWave = "";
            string text = RecFirstName(ComboBox1.SelectedItem.ToString());
            string searchFile = text + "*.*";
            string text2 = mPath + text + "\\";
            string text3 = mDestination + text + "\\";
            bool flag = false;
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = TreeViewSelected.Nodes.GetEnumerator();
                IEnumerator enumerator2 = default(IEnumerator);
                while (enumerator.MoveNext())
                {
                    TreeNode treeNode = (TreeNode)enumerator.Current;
                    try
                    {
                        enumerator2 = treeNode.Nodes.GetEnumerator();
                        while (enumerator2.MoveNext())
                        {
                            TreeNode treeNode2 = (TreeNode)enumerator2.Current;
                            string text4 = RecFirstName(treeNode2.Name);
                            NameOfWave = NameOfWave + RecLastName(treeNode2.Name) + "\r\n";
                            CopyFiles(text2 + text4, text3 + text4);
                            if (!flag)
                            {
                                flag = true;
                            }
                        }
                    }
                    finally
                    {
                        if (enumerator2 is IDisposable)
                        {
                            (enumerator2 as IDisposable).Dispose();
                        }
                    }
                }
            }
            finally
            {
                if (enumerator is IDisposable)
                {
                    (enumerator as IDisposable).Dispose();
                }
            }
            IEnumerator enumerator3 = default(IEnumerator);
            try
            {
                enumerator3 = TreeViewList.Nodes.GetEnumerator();
                IEnumerator enumerator4 = default(IEnumerator);
                while (enumerator3.MoveNext())
                {
                    TreeNode treeNode3 = (TreeNode)enumerator3.Current;
                    try
                    {
                        enumerator4 = treeNode3.Nodes.GetEnumerator();
                        while (enumerator4.MoveNext())
                        {
                            TreeNode treeNode4 = (TreeNode)enumerator4.Current;
                            string text5 = RecFirstName(treeNode4.Name);
                            if (Directory.Exists(text3 + text5))
                            {
                                Directory.Delete(text3 + text5, recursive: true);
                            }
                        }
                    }
                    finally
                    {
                        if (enumerator4 is IDisposable)
                        {
                            (enumerator4 as IDisposable).Dispose();
                        }
                    }
                }
            }
            finally
            {
                if (enumerator3 is IDisposable)
                {
                    (enumerator3 as IDisposable).Dispose();
                }
            }
            if (flag)
            {
                if (Interaction.MsgBox("Would you copy data from sources?\r\n\r\nEx: The 4 files of sources (001, 002, 003, 801)", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo, "Sources Data") == MsgBoxResult.Yes)
                {
                    CopyFiles(text2, text3, searchFile);
                }
            }
            else if (Directory.Exists(text3))
            {
                Directory.Delete(text3, recursive: true);
            }
            Interaction.MsgBox("Upgrade Completed.", MsgBoxStyle.Information);
            ToolStripStatusLabel2.Text = "";
            ToolStripProgressBar2.Value = 0;
            ButtonCreating.Enabled = true;
            ButtonZip.Enabled = true;
            ButtonUnZip.Enabled = true;
        }

        private string RecFirstName(string sValue)
        {
            int num = Strings.InStr(sValue, seprator, CompareMethod.Text);
            if (num > 0)
            {
                return Strings.Mid(sValue, 1, checked(num - 1));
            }
            return sValue;
        }

        private string RecLastName(string sValue)
        {
            int num = Strings.InStr(sValue, seprator, CompareMethod.Text);
            checked
            {
                if (num > 0)
                {
                    num += 3;
                    return Strings.Mid(sValue, num, Strings.Len(sValue) - num + 1);
                }
                return sValue;
            }
        }

        private void CopyFiles(string mSrc, string mDest, string SearchFile = "*.*", SearchOption Directory = SearchOption.TopDirectoryOnly)
        {
            string[] files = System.IO.Directory.GetFiles(mSrc, SearchFile, Directory);
            if (Operators.CompareString(Strings.Right(mSrc, 1), "\\", TextCompare: false) != 0)
            {
                mSrc += "\\";
            }
            if (Operators.CompareString(Strings.Right(mDest, 1), "\\", TextCompare: false) != 0)
            {
                mDest += "\\";
            }
            checked
            {
                int num = files.Length - 1;
                for (int i = 0; i <= num; i++)
                {
                    string text = files[i];
                    string text2 = mDest + Strings.Replace(text, mSrc, "");
                    string directory = GetDirectory(text2);
                    if (!System.IO.Directory.Exists(directory))
                    {
                        System.IO.Directory.CreateDirectory(directory);
                    }
                    int num2 = Strings.InStrRev(text2, "\\");
                    ToolStripStatusLabel2.Text = Strings.Mid(text2, num2 + 1, Strings.Len(text2) - num2);
                    ToolStripProgressBar2.Value += 1;
                    Application.DoEvents();
                    File.Copy(text, text2, overwrite: true);
                }
            }
        }

        private string GetDirectory(string mDirectory)
        {
            int num = Strings.InStrRev(mDirectory, "\\");
            if (num > 0)
            {
                return Strings.Mid(mDirectory, 1, checked(num - 1));
            }
            return mDirectory;
        }

        private int CountFiles()
        {
            string text = RecFirstName(ComboBox1.SelectedItem.ToString());
            string searchPattern = text + "*.*";
            string text2 = mPath + text + "\\";
            _ = mDestination + text + "\\";
            int num = Directory.GetFiles(text2, searchPattern, SearchOption.TopDirectoryOnly).Length;
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = TreeViewSelected.Nodes.GetEnumerator();
                IEnumerator enumerator2 = default(IEnumerator);
                while (enumerator.MoveNext())
                {
                    TreeNode treeNode = (TreeNode)enumerator.Current;
                    try
                    {
                        enumerator2 = treeNode.Nodes.GetEnumerator();
                        while (enumerator2.MoveNext())
                        {
                            TreeNode treeNode2 = (TreeNode)enumerator2.Current;
                            string text3 = RecFirstName(treeNode2.Name);
                            string[] files = Directory.GetFiles(text2 + text3, "*.*", SearchOption.TopDirectoryOnly);
                            num = checked(num + files.Length);
                        }
                    }
                    finally
                    {
                        if (enumerator2 is IDisposable)
                        {
                            (enumerator2 as IDisposable).Dispose();
                        }
                    }
                }
                return num;
            }
            finally
            {
                if (enumerator is IDisposable)
                {
                    (enumerator as IDisposable).Dispose();
                }
            }
        }

        private void TreeViewList_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Node.Level == 0)
            {
                IEnumerator enumerator = default(IEnumerator);
                try
                {
                    enumerator = e.Node.Nodes.GetEnumerator();
                    while (enumerator.MoveNext())
                    {
                        TreeNode treeNode = (TreeNode)enumerator.Current;
                        AddNodes(TreeViewSelected, treeNode.Name);
                    }
                }
                finally
                {
                    if (enumerator is IDisposable)
                    {
                        (enumerator as IDisposable).Dispose();
                    }
                }
            }
            else
            {
                TreeViewSelected.Nodes[AddNodes(TreeViewSelected, e.Node.Name)].Expand();
            }
            if (e.Node.Level == 0)
            {
                e.Node.Remove();
            }
            else if (e.Node.Parent.Nodes.Count == 1)
            {
                e.Node.Parent.Remove();
            }
            else
            {
                e.Node.Remove();
            }
        }

        private void TreeViewSelected_NodeMouseDoubleClick(object sender, TreeNodeMouseClickEventArgs e)
        {
            if (e.Node.Level == 0)
            {
                IEnumerator enumerator = default(IEnumerator);
                try
                {
                    enumerator = e.Node.Nodes.GetEnumerator();
                    while (enumerator.MoveNext())
                    {
                        TreeNode treeNode = (TreeNode)enumerator.Current;
                        AddNodes(TreeViewList, treeNode.Name);
                    }
                }
                finally
                {
                    if (enumerator is IDisposable)
                    {
                        (enumerator as IDisposable).Dispose();
                    }
                }
            }
            else
            {
                AddNodes(TreeViewList, e.Node.Name);
            }
            if (e.Node.Level == 0)
            {
                e.Node.Remove();
            }
            else if (e.Node.Parent.Nodes.Count == 1)
            {
                e.Node.Parent.Remove();
            }
            else
            {
                e.Node.Remove();
            }
        }

        private void ButtonZip_Click(object sender, EventArgs e)
        {
            ButtonZip.Enabled = false;
            ButtonCreating.Enabled = false;
            ButtonUnZip.Enabled = false;
            SaveFileDialog1.InitialDirectory = Environment.GetFolderPath(Environment.SpecialFolder.Desktop);
            SaveFileDialog1.Filter = "zip files (*.zip)|*.zip|All files (*.*)|*.*";
            SaveFileDialog1.FilterIndex = 1;
            SaveFileDialog1.RestoreDirectory = true;
            SaveFileDialog1.FileName = "Update.zip";
            SaveFileDialog1.ShowDialog();
            string fileName = SaveFileDialog1.FileName;
            ToolStripProgressBar2.Maximum = CountFiles();
            ToolStripProgressBar2.Value = 0;
            string text = RecFirstName(ComboBox1.SelectedItem.ToString());
            string searchFile = text + "*.*";
            string text2 = mPath + text + "\\";
            string text3 = mDestination + text + "\\";
            bool flag = false;
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = TreeViewSelected.Nodes.GetEnumerator();
                IEnumerator enumerator2 = default(IEnumerator);
                while (enumerator.MoveNext())
                {
                    TreeNode treeNode = (TreeNode)enumerator.Current;
                    try
                    {
                        enumerator2 = treeNode.Nodes.GetEnumerator();
                        while (enumerator2.MoveNext())
                        {
                            TreeNode treeNode2 = (TreeNode)enumerator2.Current;
                            string text4 = RecFirstName(treeNode2.Name);
                            NameOfWave = RecLastName(treeNode2.Name) + "\r\n";
                            CopyFiles(text2 + text4, text3 + text4);
                            if (!flag)
                            {
                                flag = true;
                            }
                        }
                    }
                    finally
                    {
                        if (enumerator2 is IDisposable)
                        {
                            (enumerator2 as IDisposable).Dispose();
                        }
                    }
                }
            }
            finally
            {
                if (enumerator is IDisposable)
                {
                    (enumerator as IDisposable).Dispose();
                }
            }
            IEnumerator enumerator3 = default(IEnumerator);
            try
            {
                enumerator3 = TreeViewList.Nodes.GetEnumerator();
                IEnumerator enumerator4 = default(IEnumerator);
                while (enumerator3.MoveNext())
                {
                    TreeNode treeNode3 = (TreeNode)enumerator3.Current;
                    try
                    {
                        enumerator4 = treeNode3.Nodes.GetEnumerator();
                        while (enumerator4.MoveNext())
                        {
                            TreeNode treeNode4 = (TreeNode)enumerator4.Current;
                            string text5 = RecFirstName(treeNode4.Name);
                            if (Directory.Exists(text3 + text5))
                            {
                                Directory.Delete(text3 + text5, recursive: true);
                            }
                        }
                    }
                    finally
                    {
                        if (enumerator4 is IDisposable)
                        {
                            (enumerator4 as IDisposable).Dispose();
                        }
                    }
                }
            }
            finally
            {
                if (enumerator3 is IDisposable)
                {
                    (enumerator3 as IDisposable).Dispose();
                }
            }
            if (flag)
            {
                CopyFiles(text2, text3, searchFile);
            }
            else if (Directory.Exists(text3))
            {
                Directory.Delete(text3, recursive: true);
            }
            GenerateZip(mDestination, fileName);
            Interaction.MsgBox("Zip file creation finished.\r\nZip location: \"" + fileName + "\"", MsgBoxStyle.Information);
            ToolStripStatusLabel2.Text = "";
            ToolStripProgressBar2.Value = 0;
            ButtonZip.Enabled = true;
            ButtonCreating.Enabled = true;
            ButtonUnZip.Enabled = true;
        }

        public void ExtractZip(string zipStream, string outputFolder, string RootFolder = "")
        {
            if (!Directory.Exists(outputFolder))
            {
                Directory.CreateDirectory(outputFolder);
            }
            ZipFile zipFile = new ZipFile(zipStream)
            {
                Password = ZipPassword
            };
            checked
            {
                try
                {
                    long num = zipFile.Count - 1;
                    for (long num2 = 0L; num2 <= num; num2++)
                    {
                        ZipEntry zipEntry = zipFile[(int)num2];
                        string text = zipEntry.Name;
                        if (Operators.CompareString(RootFolder, null, TextCompare: false) != 0 && Strings.InStr(text, RootFolder, CompareMethod.Text) == 1)
                        {
                            text = text.Replace(RootFolder + "/", "");
                        }
                        if (zipEntry.IsDirectory)
                        {
                            string path = Path.Combine(outputFolder, text);
                            if (!Directory.Exists(path))
                            {
                                Directory.CreateDirectory(path);
                            }
                        }
                        else if (zipEntry.IsFile)
                        {
                            string text2 = Path.Combine(outputFolder, text);
                            string directoryName = Path.GetDirectoryName(text2);
                            if (!Directory.Exists(directoryName))
                            {
                                Directory.CreateDirectory(directoryName);
                            }
                            Stream inputStream = zipFile.GetInputStream(zipEntry);
                            WriteToFile(inputStream, text2);
                            inputStream.Close();
                            File.SetCreationTime(text2, zipEntry.DateTime);
                            File.SetLastWriteTime(text2, zipEntry.DateTime);
                            File.SetLastAccessTime(text2, zipEntry.DateTime);
                        }
                    }
                }
                finally
                {
                    zipFile.Close();
                }
            }
        }

        private void WriteToFile(Stream input, string filePath)
        {
            if (File.Exists(filePath))
            {
                File.Delete(filePath);
            }
            FileStream fileStream = new FileStream(filePath, FileMode.OpenOrCreate);
            byte[] array = new byte[8193];
            int num = input.Read(array, 0, array.Length);
            while (num != 0)
            {
                fileStream.Write(array, 0, num);
                num = input.Read(array, 0, array.Length);
                Application.DoEvents();
            }
            fileStream.Close();
        }

        public void GenerateZip(string inputPath, string targetLocation)
        {
            string[] files = Directory.GetFiles(inputPath, "*.*", SearchOption.AllDirectories);
            ToolStripProgressBar2.Maximum = files.Count();
            ToolStripProgressBar2.Value = 0;
            ToolStripStatusLabel2.Text = "";
            Application.DoEvents();
            List<string[]> list = new List<string[]>();
            inputPath = Directory.GetParent(inputPath).FullName;
            checked
            {
                int startIndex = inputPath.Length + 1;
                string[] array = files;
                foreach (string text in array)
                {
                    list.Add(new string[2]
                    {
                    text,
                    text.Substring(startIndex)
                    });
                }
                if (File.Exists(targetLocation))
                {
                    File.Delete(targetLocation);
                }
                FileStream baseOutputStream = new FileStream(targetLocation, FileMode.CreateNew);
                ZipOutputStream zipOutputStream = new ZipOutputStream(baseOutputStream)
                {
                    Password = ZipPassword
                };
                foreach (string[] item in list)
                {
                    string name = item[1];
                    string text2 = item[0];
                    int num = Strings.InStrRev(text2, "\\");
                    ToolStripStatusLabel2.Text = "Zip: " + Strings.Mid(text2, num + 1, text2.Length - num);
                    Application.DoEvents();
                    byte[] array2 = File.ReadAllBytes(text2);
                    ZipEntry entry = new ZipEntry(ZipEntry.CleanName(name))
                    {
                        IsUnicodeText = true
                    };
                    zipOutputStream.PutNextEntry(entry);
                    zipOutputStream.Write(array2, 0, array2.Length);
                    zipOutputStream.CloseEntry();
                    ToolStripProgressBar2.Value += 1;
                    Application.DoEvents();
                }
                zipOutputStream.Finish();
                zipOutputStream.Close();
            }
        }

        public void TreeViewSelected_ItemDrag(object sender, ItemDragEventArgs e)
        {
            DoDragDrop(RuntimeHelpers.GetObjectValue(e.Item), DragDropEffects.Move);
        }

        public void TreeViewSelected_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent("System.Windows.Forms.TreeNode", autoConvert: true))
            {
                e.Effect = DragDropEffects.Move;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        public void TreeViewSelected_DragDrop(object sender, DragEventArgs e)
        {
            if (!e.Data.GetDataPresent("System.Windows.Forms.TreeNode", autoConvert: true))
            {
                return;
            }
            TreeView treeView = (TreeView)sender;
            TreeNode treeNode = (TreeNode)e.Data.GetData("System.Windows.Forms.TreeNode");
            _ = treeView.SelectedNode;
            if (treeNode.Level == 0)
            {
                IEnumerator enumerator = default(IEnumerator);
                try
                {
                    enumerator = treeNode.Nodes.GetEnumerator();
                    while (enumerator.MoveNext())
                    {
                        TreeNode treeNode2 = (TreeNode)enumerator.Current;
                        AddNodes(treeView, treeNode2.Name);
                    }
                }
                finally
                {
                    if (enumerator is IDisposable)
                    {
                        (enumerator as IDisposable).Dispose();
                    }
                }
            }
            else
            {
                AddNodes(treeView, treeNode.Name);
            }
            if (treeNode.Level == 0)
            {
                treeNode.Remove();
            }
            else if (treeNode.Parent.Nodes.Count == 1)
            {
                treeNode.Parent.Remove();
            }
            else
            {
                treeNode.Remove();
            }
            treeNode.EnsureVisible();
            treeView.SelectedNode = treeNode;
        }

        public void TreeViewList_ItemDrag(object sender, ItemDragEventArgs e)
        {
            DoDragDrop(RuntimeHelpers.GetObjectValue(e.Item), DragDropEffects.Move);
        }

        public void TreeViewList_DragEnter(object sender, DragEventArgs e)
        {
            if (e.Data.GetDataPresent("System.Windows.Forms.TreeNode", autoConvert: true))
            {
                e.Effect = DragDropEffects.Move;
            }
            else
            {
                e.Effect = DragDropEffects.None;
            }
        }

        public void TreeViewList_DragDrop(object sender, DragEventArgs e)
        {
            if (!e.Data.GetDataPresent("System.Windows.Forms.TreeNode", autoConvert: true))
            {
                return;
            }
            TreeView treeView = (TreeView)sender;
            TreeNode treeNode = (TreeNode)e.Data.GetData("System.Windows.Forms.TreeNode");
            _ = treeView.SelectedNode;
            if (treeNode.Level == 0)
            {
                IEnumerator enumerator = default(IEnumerator);
                try
                {
                    enumerator = treeNode.Nodes.GetEnumerator();
                    while (enumerator.MoveNext())
                    {
                        TreeNode treeNode2 = (TreeNode)enumerator.Current;
                        AddNodes(treeView, treeNode2.Name);
                    }
                }
                finally
                {
                    if (enumerator is IDisposable)
                    {
                        (enumerator as IDisposable).Dispose();
                    }
                }
            }
            else
            {
                AddNodes(treeView, treeNode.Name);
            }
            if (treeNode.Level == 0)
            {
                treeNode.Remove();
            }
            else if (treeNode.Parent.Nodes.Count == 1)
            {
                treeNode.Parent.Remove();
            }
            else
            {
                treeNode.Remove();
            }
            treeNode.EnsureVisible();
            treeView.SelectedNode = treeNode;
        }

        private void ButtonParcourir_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog1.SelectedPath = mPath;
            FolderBrowserDialog1.Description = "Please select a data source:\r\n" + mPath;
            FolderBrowserDialog1.ShowNewFolderButton = false;
            if (FolderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                mPath = FolderBrowserDialog1.SelectedPath;
                if (Operators.CompareString(Strings.Right(mPath, 1), "\\", TextCompare: false) != 0)
                {
                    mPath += "\\";
                }
                ReadSources();
            }
        }
    }

}
