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
using System.Text;
using System.Windows.Forms;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;


namespace Facility_Management
{

    public partial class DataRecoveryP : Form
    {
        public delegate void MyDelegate(ComboBox myControl, ArrayList myArg2);

        private string mPath;

        private string mDestination;

        public bool LaunchDirectClass;

        public string NameOfWave;

        public bool IsNotLockedTreeView;

        private readonly Dictionary<string, string> databasePresse;

        public DataRecoveryP()
        {
            base.Load += Form1_Load;
            IsNotLockedTreeView = true;
            databasePresse = new Dictionary<string, string>();
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
        }

        private void ComboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            ButtonCreating.Enabled = false;
            string text = Conversions.ToString(ComboBox1.SelectedItem).Trim();
            checked
            {
                if (Operators.CompareString(text, null, TextCompare: false) != 0)
                {
                    TreeViewList.Nodes.Clear();
                    TreeViewSelected.Nodes.Clear();
                    TreeViewList.Sort();
                    TreeViewSelected.Sort();
                    databasePresse.Clear();
                    Application.DoEvents();
                    string[] directories = Directory.GetDirectories(mPath, "*.", SearchOption.TopDirectoryOnly);
                    int num = directories.Length - 1;
                    for (int i = 0; i <= num; i++)
                    {
                        if (!Directory.Exists(directories[i] + "\\DroitsAcces\\" + text))
                        {
                            continue;
                        }
                        string text2 = mFunction.ParsePath(directories[i], 4);
                        string text3 = ReadSourceName(text2);
                        if (Operators.CompareString(text2, "", TextCompare: false) != 0)
                        {
                            TreeView node = ((!Directory.Exists(mDestination + text2)) ? TreeViewList : TreeViewSelected);
                            if (Operators.CompareString(text3, "", TextCompare: false) != 0)
                            {
                                AddNodes(node, text3);
                            }
                        }
                    }
                }
                ButtonCreating.Enabled = true;
            }
        }

        public void ReadDroitAcces()
        {
            string[] directories = Directory.GetDirectories(mPath, "DroitsAcces", SearchOption.AllDirectories);
            ArrayList arrayList = new ArrayList();
            checked
            {
                int num = directories.Length - 1;
                int num2 = 0;
                while (true)
                {
                    if (num2 <= num)
                    {
                        string[] directories2 = Directory.GetDirectories(directories[num2], "*.*", SearchOption.TopDirectoryOnly);
                        foreach (string szPath in directories2)
                        {
                            if (base.Visible)
                            {
                                string text = mFunction.ParsePath(szPath, 4);
                                if (!arrayList.Contains(text))
                                {
                                    arrayList.Add(text);
                                }
                                continue;
                            }
                            return;
                        }
                        num2++;
                        continue;
                    }
                    object[] args = new object[2] { ComboBox1, arrayList };
                    ComboBox1.Invoke(new MyDelegate(DelegateMethod), args);
                    break;
                }
            }
        }

        public void DelegateMethod(ComboBox myControl, ArrayList ListDroitAcces)
        {
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = ListDroitAcces.GetEnumerator();
                while (enumerator.MoveNext())
                {
                    string value = Conversions.ToString(enumerator.Current);
                    if (!base.Visible)
                    {
                        break;
                    }
                    if (!myControl.Items.Contains(Strings.UCase(value)))
                    {
                        myControl.Items.Add(Strings.UCase(value));
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
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            Init();
            base.Opacity = MyProject.Forms.FormBox.nvOpacity;
            ComboBox1.Items.Clear();
            ComboBox1.Sorted = true;
            ComboBox1.Items.Add("JFC");
            ToolStripStatusLabel1.Text = "";
            new Action(ReadDroitAcces).BeginInvoke(null, null);
        }

        private string ReadSourceName(string SourceName)
        {
            string[] array = File.ReadAllLines(mPath + "Sources.tbl", Encoding.Default);
            string[] array2 = File.ReadAllLines(mPath + "Terrains.tbl", Encoding.Default);
            string text = "";
            string[] array3 = array2;
            foreach (string text2 in array3)
            {
                if (text2.Contains("/" + SourceName + "/"))
                {
                    text = text2;
                    break;
                }
            }
            string text3 = "";
            if (Operators.CompareString(text, null, TextCompare: false) != 0)
            {
                string[] array4 = Strings.Split(text, ",");
                string[] array5 = array;
                foreach (string expression in array5)
                {
                    try
                    {
                        string[] array6 = Strings.Split(expression, ",");
                        if (array6.Length == 3 && Operators.CompareString(array6[0].Trim(), array4[1].Trim(), TextCompare: false) == 0)
                        {
                            text3 = Strings.Replace(array6[2], "\"", "").Trim();
                            text = Strings.Replace(array4[3], "\"", "").Trim();
                            goto IL_0136;
                        }
                    }
                    catch (Exception ex)
                    {
                        ProjectData.SetProjectError(ex);
                        Exception ex2 = ex;
                        ProjectData.ClearProjectError();
                    }
                }
            }
            goto IL_0136;
        IL_0136:
            if ((Operators.CompareString(text3, "", TextCompare: false) != 0) & (Operators.CompareString(text, "", TextCompare: false) != 0))
            {
                databasePresse.Add(text3 + "\\" + text, SourceName);
                return text3 + "\\" + text;
            }
            return "";
        }

        private void AddNodes(TreeView node, string Libelle)
        {
            int num = Strings.InStr(Libelle, "\\", CompareMethod.Text);
            checked
            {
                if (num > 0)
                {
                    string key = Strings.Mid(Libelle, 1, num - 1);
                    Libelle = Strings.Mid(Libelle, num + 1, Strings.Len(Libelle) - 1);
                    if (!node.Nodes.ContainsKey(key))
                    {
                        node.Nodes.Add(key, key);
                    }
                    int index = node.Nodes.IndexOfKey(key);
                    if (!node.Nodes[index].Nodes.ContainsKey(Libelle))
                    {
                        node.Nodes[index].Nodes.Add(Libelle, Libelle);
                    }
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
                        AddNodes(TreeViewSelected, e.Node.Name + "\\" + treeNode.Name);
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
                AddNodes(TreeViewSelected, e.Node.Parent.Name + "\\" + e.Node.Name);
                TreeViewSelected.Nodes[TreeViewSelected.Nodes.IndexOfKey(e.Node.Parent.Text)].Expand();
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
                        AddNodes(TreeViewList, e.Node.Name + "\\" + treeNode.Name);
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
                AddNodes(TreeViewList, e.Node.Parent.Name + "\\" + e.Node.Name);
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
                        AddNodes(treeView, treeNode.Name + "\\" + treeNode2.Name);
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
                AddNodes(treeView, treeNode.Parent.Name + "\\" + treeNode.Name);
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
                        AddNodes(treeView, treeNode.Name + "\\" + treeNode2.Name);
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
                AddNodes(treeView, treeNode.Parent.Name + "\\" + treeNode.Name);
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
                    string path = mFunction.ParsePath(text2, 6);
                    if (!System.IO.Directory.Exists(path))
                    {
                        System.IO.Directory.CreateDirectory(path);
                    }
                    int num2 = Strings.InStrRev(text2, "\\");
                    ToolStripStatusLabel1.Text = Strings.Mid(text2, num2 + 1, Strings.Len(text2) - num2);
                    ToolStripProgressBar1.Value += 1;
                    Application.DoEvents();
                    File.Copy(text, text2, overwrite: true);
                }
            }
        }

        private int CountFiles()
        {
            string text = mPath + "\\";
            int num = 0;
            checked
            {
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
                                string text2 = databasePresse[treeNode.Name + "\\" + treeNode2.Name];
                                string[] files = Directory.GetFiles(text + text2, "*.*", SearchOption.TopDirectoryOnly);
                                num += files.Length;
                                string[] files2 = Directory.GetFiles(text + text2 + "\\DroitsAcces\\" + Conversions.ToString(ComboBox1.SelectedItem).Trim(), "*.*", SearchOption.TopDirectoryOnly);
                                num += files2.Length;
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
        }

        private void ButtonCreating_Click(object sender, EventArgs e)
        {
            ButtonCreating.Enabled = false;
            ToolStripProgressBar1.Maximum = CountFiles();
            ToolStripProgressBar1.Value = 0;
            File.Delete(mDestination + "Sources.new");
            File.Delete(mDestination + "Terrains.new");
            NameOfWave = "Terrains:\r\n";
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
                            string text = databasePresse[treeNode.Name + "\\" + treeNode2.Name];
                            CopyFiles(mPath + text, mDestination + text);
                            CopyFiles(mPath + text + "\\DroitsAcces\\" + Conversions.ToString(ComboBox1.SelectedItem).Trim(), mDestination + text);
                            AddLineToFile(mDestination + "Sources.new", FindSourceTbl(treeNode.Name));
                            AddLineToFile(mDestination + "Terrains.new", FindTerrainTbl(text));
                            NameOfWave = NameOfWave + "- " + treeNode.Name + " " + treeNode2.Name + "\r\n";
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
                            string text2 = databasePresse[treeNode3.Name + "\\" + treeNode4.Name];
                            if (Directory.Exists(mDestination + text2))
                            {
                                Directory.Delete(mDestination + text2, recursive: true);
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
            File.Copy(mPath + "Terrains.tbl", mDestination + "Terrains.maj", overwrite: true);
            File.Copy(mPath + "Sources.tbl", mDestination + "Sources.maj", overwrite: true);
            Interaction.MsgBox("Upgrade Completed.", MsgBoxStyle.Information);
            ToolStripStatusLabel1.Text = "";
            ToolStripProgressBar1.Value = 0;
            ButtonCreating.Enabled = true;
        }

        private void ButtonExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        private string FindTerrainTbl(string TerrainName)
        {
            string[] array = File.ReadAllLines(mPath + "Terrains.tbl", Encoding.Default);
            foreach (string text in array)
            {
                if (text.Contains("/" + TerrainName + "/"))
                {
                    return text;
                }
            }
            return "";
        }

        private string FindSourceTbl(string SourceName)
        {
            string[] array = File.ReadAllLines(mPath + "Sources.tbl", Encoding.Default);
            foreach (string text in array)
            {
                if (text.Contains("\"" + SourceName + "\""))
                {
                    return text;
                }
            }
            return "";
        }

        public void AddLineToFile(string szFile, string svAddLine)
        {
            if (Operators.CompareString(svAddLine, "", TextCompare: false) == 0)
            {
                return;
            }
            checked
            {
                if (File.Exists(szFile))
                {
                    string[] array = File.ReadAllLines(szFile, Encoding.Default);
                    if (!array.Contains(svAddLine))
                    {
                        int i;
                        for (i = array.Length - 1; i >= 0 && Operators.CompareString(array[i].Trim(), null, TextCompare: false) == 0; i += -1)
                        {
                        }
                        array = (string[])Utils.CopyArray(array, new string[i + 1 + 1]);
                        array[i + 1] = svAddLine;
                        File.WriteAllLines(szFile, array, Encoding.Default);
                    }
                }
                else
                {
                    File.WriteAllText(szFile, svAddLine, Encoding.Default);
                }
            }
        }
    }

}
