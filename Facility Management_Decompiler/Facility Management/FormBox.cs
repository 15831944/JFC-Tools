using Facility_Management.My;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data.SqlClient;
using System.Diagnostics;
using System.Drawing;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Windows.Forms;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;
using Facility_Management.Properties;

namespace Facility_Management
{

    internal partial class FormBox : Form
    {
        public struct PATCH_INFO
        {
            public string count;

            public string patch;

            public string size;

            public string crc;

            public string info;

            public override string ToString()
            {
                return count + "," + patch + "," + size + "," + crc + "," + info;
            }

            public string FindString(string[] stab)
            {
                foreach (string text in stab)
                {
                    if ((Operators.CompareString(text, BaliseAccesEndOfFile, TextCompare: false) == 0) | (Operators.CompareString(text, null, TextCompare: false) == 0))
                    {
                        break;
                    }
                    if (Operators.CompareString(Strings.Split(text, ",")[1], patch, TextCompare: false) == 0)
                    {
                        return text.Split(',')[0].Trim();
                    }
                }
                return null;
            }
        }

        public struct MAP_DRIVE
        {
            public string LETTER;

            public string PATH;

            public string TEST;

            public bool Value(string svString)
            {
                string[] array = Strings.Split(svString, ",");
                if (array.Length == 3)
                {
                    LETTER = array[0];
                    PATH = array[1];
                    TEST = array[2];
                    return true;
                }
                LETTER = null;
                PATH = null;
                TEST = null;
                return false;
            }
        }

        private readonly DateTime DIndex;

        public string FacilityManagementIni;

        public string mPathExes;

        public string mPathAcces;

        public string mExtFileAcces;

        public string mExtFileExes;

        public string mExtFileZips;

        public string mExtFileInfo;

        public string mExtFileCRC;

        public string mExtFileTmp;

        public string MenuPays;

        public string MenuSoft;

        public bool FilterSupp;

        public bool bUseShellExecute;

        public bool bAdvancedMenu;

        public bool bInsufficientDiskSpace;

        public bool ListAccesClient_mDown;

        public bool ListSelectClient_mDown;

        public bool ListSelectPatch_mDown;

        public bool ListPatch_mDown;

        public static string BaliseAccesEndOfFile = "##";

        public static string NewBaliseAccesEndOfFile = "###";

        public static string OldBaliseAccesEndOfFile = "##";

        public static string Balise2AccesEndOfFile = "#END#";

        public static string BaliseCRCAccesFile = "%CRC%";

        public static string BaliseUpdateEndOfFile = "##";

        public string Skip;

        public string Adm;

        public string svAppBuild;

        public string ISPROJECTDIR;

        public string DATAFRET;

        public string DATA;

        private string[] ListAcces;

        public Dictionary<string, bool> DicExes;

        public Dictionary<string, Image> DicPaysImage;

        public Dictionary<string, Image> DicSoftImage;

        public Dictionary<string, string> CountryDic;

        public Dictionary<string, string> SoftwareDic;

        public Dictionary<string, int> cMSDic;

        public Dictionary<string, string> AliasDic;

        public double nvOpacity;

        public Color backcolor1;

        public Color backcolor2;

        public Color backcolor3;

        private string SQL_Serveur;

        private string SQL_Login;

        private string SQL_Password;

        private bool SQL_NoErrorMessage;

        private string SQL_SiteInternet_Serveur;

        private readonly string SQL_SiteInternet_Login;

        private readonly string SQL_SiteInternet_Password;

        public FormBox()
        {
            base.Load += Form1_Load;
            base.Resize += FormBox_Resize;
            DIndex = Conversions.ToDate(mConnection.Cal);
            mExtFileAcces = ".acces";
            mExtFileExes = ".exe";
            mExtFileZips = ".zip";
            mExtFileInfo = ".txt";
            mExtFileCRC = ".crc";
            mExtFileTmp = ".tmp";
            MenuPays = "";
            MenuSoft = "";
            FilterSupp = false;
            bUseShellExecute = false;
            //bAdvancedMenu = false;
            bAdvancedMenu = true;
            bInsufficientDiskSpace = false;
            ListAccesClient_mDown = false;
            ListSelectClient_mDown = false;
            ListSelectPatch_mDown = false;
            ListPatch_mDown = false;
            Skip = "[SKIP]";
            Adm = "[ADM]";
            svAppBuild = null;
            ISPROJECTDIR = null;
            DATAFRET = null;
            DATA = null;
            DicExes = new Dictionary<string, bool>();
            DicPaysImage = new Dictionary<string, Image>();
            DicSoftImage = new Dictionary<string, Image>();
            CountryDic = new Dictionary<string, string>();
            SoftwareDic = new Dictionary<string, string>();
            cMSDic = new Dictionary<string, int>();
            AliasDic = new Dictionary<string, string>();
            backcolor1 = Color.LightBlue;
            backcolor2 = Color.AliceBlue;
            backcolor3 = Color.SteelBlue;
            SQL_NoErrorMessage = false;
            SQL_SiteInternet_Serveur = "172.17.230.209";
            SQL_SiteInternet_Login = "jfcupdate";
            SQL_SiteInternet_Password = "P@ssword02";
            InitializeComponent();
        }

        private string AliasCompany(string Name)
        {
            string NomModule = "Alias";
            string text = mFileIni.Select_GetIniString(ref NomModule, ref Name, ref FacilityManagementIni).Trim();
            if (Operators.CompareString(text, null, TextCompare: false) == 0)
            {
                text = Name;
            }
            AliasDic.Add(text, Name);
            return text;
        }

        public void GetCustomerOfCountry(bool F5 = false)
        {
            ListAccesClient.Items.Clear();
            ListSelectClient.Items.Clear();
            Label1.Text = "Customer List:";
            if (Directory.Exists(mPathAcces))
            {
                ListAcces = Directory.GetFiles(mPathAcces, "*" + mExtFileAcces, SearchOption.TopDirectoryOnly);
                ListAcces = (from x in ListAcces
                             select (x) into x
                             orderby x
                             select x).ToArray();
                ListAcces = ListAcces.Select((string x) => mFunction.ParsePath(x, 5)).ToArray();
                AliasDic.Clear();
                string[] listAcces = ListAcces;
                foreach (string name in listAcces)
                {
                    ListAccesClient.Items.Add(AliasCompany(name));
                }
                if (!F5)
                {
                    DelegatCheckClients();
                }
                Label1.Text = "Customer List (" + Conversions.ToString(ListAccesClient.Items.Count) + "):";
                Label2.Text = "Selected customers (" + Conversions.ToString(ListSelectClient.Items.Count) + "):";
            }
        }

        public void DelegatCheckClients()
        {
            new Action(CheckClients).BeginInvoke(null, null);
        }

        private string ConvPays()
        {
            return MenuPays;
        }

        public void CreateSocieteAuto(string svDuplicateClient = null)
        {
            Dictionary<string, string> dictionary = ReadSqlCreate();
            if (dictionary.Count == 0)
            {
                return;
            }
            int num = 2;
            int num2 = default(int);
            foreach (string key in dictionary.Keys)
            {
                if (Strings.StrComp(dictionary[key], MenuPays, CompareMethod.Text) != 0 || AliasDic.ContainsValue(key))
                {
                    continue;
                }
                if (num != 6)
                {
                    num2 = (int)Interaction.MsgBox("New Client: \"" + key + "\" ?", MsgBoxStyle.YesNoCancel | MsgBoxStyle.Question, "Facility Management - " + MenuSoft + " - " + MenuPays);
                }
                switch (num2)
                {
                    case 6:
                        if (dictionary.Count > 1 && num == 2)
                        {
                            num = (int)Interaction.MsgBox("Voulez-vous faire de même pour les autres clients ?", MsgBoxStyle.YesNo | MsgBoxStyle.Question, "Facility Management - " + MenuSoft + " - " + MenuPays);
                        }
                        if (Operators.CompareString(svDuplicateClient, null, TextCompare: false) != 0)
                        {
                            string text = mPathAcces + key + mExtFileAcces;
                            File.Copy(mPathAcces + svDuplicateClient + mExtFileAcces, text, overwrite: true);
                            DateTime now = DateAndTime.Now;
                            File.SetCreationTime(text, now);
                            File.SetLastAccessTime(text, now);
                            File.SetLastWriteTime(text, now);
                        }
                        else
                        {
                            string path = mPathAcces + key + mExtFileAcces;
                            if (File.Exists(path))
                            {
                                File.Delete(path);
                            }
                            File.WriteAllText(path, BaliseAccesEndOfFile, Encoding.Default);
                        }
                        ListAccesClient.Items.Add(AliasCompany(key));
                        break;
                    case 7:
                        num = 7;
                        break;
                    case 2:
                        return;
                }
            }
        }

        public void CheckClients()
        {
            Dictionary<string, string> dictionary = ReadSql();
            if (dictionary.Count == 0)
            {
                return;
            }
            string text = null;
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = ListAccesClient.Items.GetEnumerator();
                while (enumerator.MoveNext())
                {
                    object objectValue = RuntimeHelpers.GetObjectValue(enumerator.Current);
                    if (!dictionary.Keys.Contains(AliasDic[objectValue.ToString()]) & !objectValue.ToString().Contains("JFC Test"))
                    {
                        text = ((Operators.CompareString(text, null, TextCompare: false) == 0) ? ("- " + objectValue.ToString()) : (text + "\r\n- " + objectValue.ToString()));
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
            if (Operators.CompareString(text, null, TextCompare: false) != 0)
            {
                text = "List of customers not created in the database of serial numbers:\r\n" + text;
            }
            string text2 = null;
            foreach (string key in dictionary.Keys)
            {
                if (Operators.CompareString(dictionary[key], MenuPays, TextCompare: false) == 0 && !ListAcces.Contains(key))
                {
                    text2 = ((Operators.CompareString(text2, null, TextCompare: false) == 0) ? ("- " + key) : (text2 + "\r\n- " + key));
                }
            }
            if (Operators.CompareString(text2, null, TextCompare: false) != 0)
            {
                text2 = "List of customers not created in the access files:\r\n" + text2;
                text = ((Operators.CompareString(text, null, TextCompare: false) == 0) ? text2 : (text + "\r\n\r\n" + text2));
            }
            if (Operators.CompareString(text, null, TextCompare: false) != 0)
            {
                Interaction.MsgBox(text, MsgBoxStyle.Exclamation, "Facility Management - " + MenuSoft + " - " + MenuPays);
            }
        }

        private void InitImage()
        {
            CustomerMenu.Image = Resources.utilisateurs;
            ExitMenu.Image = Resources.sortie;
            CountryMenu.Image = Resources.avertissement;
            SoftwareMenu.Image = Resources.disquedur;
            UpdateMenu.Image = Resources.Maj;
            DicPaysImage.Clear();
            foreach (string key in CountryDic.Keys)
            {
                string text = mFunction.ParsePath(Application.ExecutablePath, 6) + "\\Ressources\\" + key + ".png";
                if (File.Exists(text))
                {
                    Bitmap value = new Bitmap(text);
                    DicPaysImage.Add(key, value);
                }
                else
                {
                    DicPaysImage.Add(key, Resources.avertissement);
                }
            }
            DicSoftImage.Clear();
            foreach (string key2 in SoftwareDic.Keys)
            {
                string text2 = mFunction.ParsePath(Application.ExecutablePath, 6) + "\\Ressources\\" + key2 + ".ico";
                if (File.Exists(text2))
                {
                    Icon icon = new Icon(text2, new Size(32, 32));
                    DicSoftImage.Add(key2, icon.ToBitmap());
                }
                else
                {
                    DicSoftImage.Add(key2, Resources.avertissement);
                }
            }
            CountryMenu.DisplayStyle = ToolStripItemDisplayStyle.ImageAndText;
            SoftwareMenu.DisplayStyle = ToolStripItemDisplayStyle.ImageAndText;
        }

        private void InitMenu()
        {
            string NomModule = "Parametres";
            string MotCle = "Country";
            string[] array = Strings.Split(mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref FacilityManagementIni), ",");
            CountryDic.Clear();
            SoftwareDic.Clear();
            checked
            {
                int num = array.Length - 1;
                for (int i = 0; i <= num; i += 2)
                {
                    CountryDic.Add(array[i].Trim(), array[i + 1].Trim() + "\\");
                }
                MotCle = "Parametres";
                NomModule = "Software";
                string[] array2 = (from x in Strings.Split(mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref FacilityManagementIni), ",")
                                   select x.Trim() into x
                                   orderby x
                                   select x).ToArray();
                string[] source = new string[5] { "Medial 2", "AutoUpdate", "Facility Management", "MultiUser", "Services" };
                string[] array3 = array2;
                for (int j = 0; j < array3.Length; j++)
                {
                    string NomModule2 = array3[j];
                    if (!source.Contains(NomModule2) | bAdvancedMenu)
                    {
                        Dictionary<string, string> softwareDic = SoftwareDic;
                        string key = NomModule2;
                        NomModule = "PathExes";
                        softwareDic.Add(key, mFileIni.Select_GetIniString(ref NomModule2, ref NomModule, ref FacilityManagementIni).Trim());
                    }
                }
                ListAccesClient.Sorted = true;
                ListSelectClient.Sorted = true;
                SoftwareMenu = new ToolStripMenuItem();
                CountryMenu = new ToolStripMenuItem();
                CustomerMenu = new ToolStripMenuItem();
                UpdateMenu = new ToolStripMenuItem();
                ToolsMenu = new ToolStripMenuItem();
                ExitMenu = new ToolStripMenuItem();
                UnderMenuService = new ToolStripMenuItem();
                AddMenuService = new ToolStripMenuItem();
                DelMenuService = new ToolStripMenuItem();
                SearchMenuService = new ToolStripMenuItem();
                MenuStripFM.Items.AddRange(new ToolStripItem[6] { SoftwareMenu, CountryMenu, CustomerMenu, UpdateMenu, ToolsMenu, ExitMenu });
                cMSDic.Clear();
                cMSDic.Add("New/Created", 0);
                cMSDic.Add("Edit selection", 1);
                cMSDic.Add("Delete selection", 2);
                cMSDic.Add("Add selection", 3);
                cMSDic.Add("Add all", 4);
                cMSDic.Add("Remove selection", 5);
                cMSDic.Add("Remove all selection", 6);
                cMSDic.Add("Search in customer", 7);
                cMSDic.Add("Duplicate customer", 8);
                cMSDic.Add("Add patchs from customers", 9);
                cMSDic.Add("Send FTP", 10);
                cMSDic.Add("Get data country", 11);
                cMSDic.Add("Create patch", 12);
                cMSDic.Add("Delete patch from customers", 13);
                cMSDic.Add("Add all patchs from customer", 14);
                cMSDic.Add("Erase all data country", 15);
                cMSDic.Add("UTF7 to default", 16);
                cMSDic.Add("UTF7 to default (All)", 17);
                cMSDic.Add("Send patchs automatically (Pige)", 18);
                cMSDic.Add("Refresh", 19);
                cMSDic.Add("Select all customer Pige", 20);
                cMSDic.Add("Invert selection", 21);
                cMSDic.Add("Request info", 22);
                cMSDic.Add("Kill process", 23);
                cMSDic.Add("Request log", 24);
                cMSDic.Add("Advanced options", 25);
                cMSDic.Add("System Info", 26);
                cMSDic.Add("Configuration.xml", 27);
                cMSDic.Add("Medial Supervisor", 28);
                cMSDic.Add("Services Status", 29);
                cMSDic.Add("Ping", 30);
                cMSDic.Add("Delete all patch before from customers", 31);
                cMSDic.Add("Reindexing", 32);
                cMSDic.Add("Add Group", 33);
                cMSDic.Add("Alias", 34);
                cMSDic.Add("Search in database", 35);
                cMSDic.Add("Export", 36);
                cMSDic.Add("No Export", 37);
                cMSDic.Add("Force wait", 38);
                cMSDic.Add("Force ID", 39);
                cMSDic.Add("New/Created (Auto)", 40);
                cMSDic.Add("Import", 41);
                cMSDic.Add("Couper", 42);
                cMSDic.Add("Add patchs from customers (Send)", 43);
                cMSDic.Add("Creating CRC file", 44);
                cMSDic.Add("Tasklist", 45);
                cMSDic.Add("Session", 46);
                cMSDic.Add("TaskKill", 47);
                cMSDic.Add("Rename", 48);
                cMSDic.Add("Skip patch from customers", 49);
                cMSDic.Add("Not skip patch from customers", 50);
                cMSDic.Add("Proxy", 51);
                cMSDic.Add("Admin patch from customers", 52);
                cMSDic.Add("Not admin patch from customers", 53);
                cMSDic.Add("Balise " + NewBaliseAccesEndOfFile, 54);
                cMSDic.Add("Balise " + OldBaliseAccesEndOfFile, 55);
                cMSDic.Add("Balise " + Balise2AccesEndOfFile, 56);
                cMSDic.Add("Add CRC", 57);
                cMSDic.Add("Add All CRC", 58);
                cMSDic.Add("%SYNCHRONIZE%", 59);
                cMSDic.Add("%TESTUPDATE%1", 60);
                cMSDic.Add("%TESTUPDATE%0", 61);
                cMSDic.Add("%REDISTRIBUABLE%14", 62);
                cMSDic.Add("%FORCEUPDATE%", 63);
                cMSDic.Add("%TESTUPDATE%", 64);
                cMSDic.Add("Loading patch from file", 65);
                CMS_ListAccesClient.Items.Add("New");
                CMS_ListPatch.Items.Add("New");
                CMS_ListSelectClient.Items.Add("New");
                CMS_ListSelectPatch.Items.Add("New");
                ToolStripStatusLabelFM.Text = "";
                MenuStripFM.Size = new Size(base.Size.Width - 16, 40);
                CountryMenu.Text = "Country";
                CountryMenu.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                CountryMenu.ImageScaling = ToolStripItemImageScaling.None;
                CountryMenu.AutoSize = false;
                SoftwareMenu.Text = "Software";
                SoftwareMenu.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                SoftwareMenu.ImageScaling = ToolStripItemImageScaling.None;
                SoftwareMenu.AutoSize = false;
                CustomerMenu.Text = "Customer";
                CustomerMenu.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                CustomerMenu.ImageScaling = ToolStripItemImageScaling.None;
                CustomerMenu.AutoSize = false;
                UpdateMenu.Text = "Patch";
                UpdateMenu.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                UpdateMenu.ImageScaling = ToolStripItemImageScaling.None;
                UpdateMenu.AutoSize = false;
                ToolsMenu.Text = "Tools";
                ToolsMenu.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                ToolsMenu.ImageScaling = ToolStripItemImageScaling.None;
                ToolsMenu.AutoSize = false;
                ExitMenu.Text = "Exit";
                ExitMenu.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                ExitMenu.ImageScaling = ToolStripItemImageScaling.None;
                ExitMenu.AutoSize = false;
                FormBoxResize();
                CustomerMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(9));
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].Image = Resources.telecharger;
                CustomerMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(43));
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].Image = Resources.telecharger;
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].Visible = bAdvancedMenu;
                CustomerMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(13));
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].Image = Resources.Undo;
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].Visible = bAdvancedMenu;
                CustomerMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(31));
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].Image = Resources.conversion;
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].Visible = bAdvancedMenu;
                CustomerMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(32));
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].Image = Resources.index;
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.Count - 1].Visible = bAdvancedMenu;
                UpdateMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(10));
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Image = Resources.ftp;
                UpdateMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(44));
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Image = Resources.conversion;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Visible = bAdvancedMenu;
                UpdateMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(49));
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].ForeColor = Color.Green;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Font = new Font("Tahoma", 14f, FontStyle.Italic, GraphicsUnit.Point, 0);
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Image = Resources.dernier;
                UpdateMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(50));
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].ForeColor = Color.Green;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Font = new Font("Tahoma", 14f, FontStyle.Italic, GraphicsUnit.Point, 0);
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Image = Resources.premier;
                UpdateMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(52));
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].ForeColor = Color.Red;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Font = new Font("Tahoma", 14f, FontStyle.Italic, GraphicsUnit.Point, 0);
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Image = Resources.dernier;
                UpdateMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(53));
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].ForeColor = Color.Red;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Font = new Font("Tahoma", 14f, FontStyle.Italic, GraphicsUnit.Point, 0);
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Image = Resources.premier;
                UpdateMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(65));
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].ForeColor = Color.Purple;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Font = new Font("Tahoma", 14f, FontStyle.Italic, GraphicsUnit.Point, 0);
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Image = Resources.import;
                UpdateMenu.DropDownItems[UpdateMenu.DropDownItems.Count - 1].Visible = bAdvancedMenu;
                ToolsMenu.Image = Resources.tools;
                ToolsMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(19));
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Image = Resources.reload;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(19);
                ToolsMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(12));
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Image = Resources.Package2;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(12);
                ToolsMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(11));
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Image = Resources.ImportData;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(11);
                ToolsMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(18));
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Image = Resources.Wizard_Wand;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(18);
                ToolsMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(15));
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Image = Resources.corbeille;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(15);
                ToolsMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(20));
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Image = Resources.Wizard_Wand;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(20);
                ToolsMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(21));
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Image = Resources._Auto;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(21);
                ToolsMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(45));
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Image = Resources.radar_wifi_icone;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Visible = bAdvancedMenu;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(45);
                ToolsMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(46));
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Image = Resources.personne;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Visible = bAdvancedMenu;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(46);
                ToolsMenu.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(47));
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Image = Resources.Delete;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Visible = bAdvancedMenu;
                ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(47);
                CustomerMenu.DropDownItems.AddRange(new ToolStripItem[1] { UnderMenuService });
                UnderMenuService.Text = cMSDic.Keys.ElementAtOrDefault(25);
                UnderMenuService.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                UnderMenuService.ImageScaling = ToolStripItemImageScaling.None;
                UnderMenuService.Image = Resources.tools;
                UnderMenuService.AutoSize = true;
                UnderMenuService.Name = cMSDic.Keys.ElementAtOrDefault(25);
                UnderMenuService.DropDownItems.AddRange(new ToolStripItem[3] { AddMenuService, DelMenuService, SearchMenuService });
                AddMenuService.Text = cMSDic.Keys.ElementAtOrDefault(3);
                AddMenuService.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                AddMenuService.ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.Image = Resources.Add;
                AddMenuService.AutoSize = true;
                AddMenuService.Name = cMSDic.Keys.ElementAtOrDefault(3);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(22));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.Info;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(22);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(23));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.Services;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(23);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(24));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.zip2;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(24);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(26));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.system;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(26);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(27));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.xml;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(27);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(28));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.Supervisor;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(28);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(29));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.status;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(29);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(30));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.graphique;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(30);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(4));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.Ajouter;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(4);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(36));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.document_export;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(36);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(37));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.document_noexport;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(37);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(38));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.wait;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(38);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(39));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.utilisateur;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(39);
                AddMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(51));
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Image = Resources.globe1;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(51);
                DelMenuService.Text = cMSDic.Keys.ElementAtOrDefault(2);
                DelMenuService.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                DelMenuService.ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.Image = Resources.Delete;
                DelMenuService.AutoSize = true;
                DelMenuService.Name = cMSDic.Keys.ElementAtOrDefault(2);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(22));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.Info;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(22);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(23));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.Services;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(23);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(24));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.zip2;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(24);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(26));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.system;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(26);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(27));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.xml;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(27);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(28));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.Supervisor;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(28);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(29));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.status;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(29);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(30));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.graphique;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(30);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(6));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.Services;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(6);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(36));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.document_export;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(36);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(37));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.document_noexport;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(37);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(38));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.wait;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(38);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(39));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.utilisateur;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(39);
                DelMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(51));
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Image = Resources.globe1;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(51);
                SearchMenuService.Text = cMSDic.Keys.ElementAtOrDefault(7);
                SearchMenuService.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                SearchMenuService.ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.Image = Resources.Preview;
                SearchMenuService.AutoSize = true;
                SearchMenuService.Name = cMSDic.Keys.ElementAtOrDefault(7);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(22));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.Info;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(22);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(23));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.Services;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(23);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(24));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.zip2;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(24);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(26));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.system;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(26);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(27));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.xml;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(27);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(28));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.Supervisor;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(28);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(29));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.status;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(29);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(30));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.graphique;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(30);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(7));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.find;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(7);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(36));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.document_export;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(36);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(37));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.document_noexport;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(37);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(38));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.wait;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(38);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(39));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.utilisateur;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(39);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(51));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.globe1;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(51);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(59));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.wait;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(59);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(60));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.graphique;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(60);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(61));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.graphique;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(61);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(63));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.Maj;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(63);
                SearchMenuService.DropDownItems.Add(cMSDic.Keys.ElementAtOrDefault(64));
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Image = Resources.gnome;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.Count - 1].Name = cMSDic.Keys.ElementAtOrDefault(64);
                CMS_ListAccesClient.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                CMS_ListPatch.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                CMS_ListSelectClient.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                CMS_ListSelectPatch.Font = new Font("Tahoma", 14f, FontStyle.Regular, GraphicsUnit.Point, 0);
                CountryMenu.BackColor = backcolor3;
                SoftwareMenu.BackColor = backcolor3;
                ExitMenu.BackColor = backcolor3;
                UpdateMenu.BackColor = backcolor3;
                ToolsMenu.BackColor = backcolor3;
                CustomerMenu.BackColor = backcolor3;
                UnderMenuService.BackColor = backcolor3;
                CustomerMenu.BackColor = backcolor3;
                CMS_ListAccesClient.BackColor = backcolor1;
                CMS_ListPatch.BackColor = backcolor1;
                CMS_ListSelectClient.BackColor = backcolor1;
                CMS_ListSelectPatch.BackColor = backcolor1;
                int num2 = ToolsMenu.DropDownItems.Count - 1;
                for (int k = 0; k <= num2; k++)
                {
                    ToolsMenu.DropDownItems[k].BackColor = backcolor2;
                }
                int num3 = UpdateMenu.DropDownItems.Count - 1;
                for (int l = 0; l <= num3; l++)
                {
                    UpdateMenu.DropDownItems[l].BackColor = backcolor2;
                }
                int num4 = CustomerMenu.DropDownItems.Count - 1;
                for (int m = 0; m <= num4; m++)
                {
                    CustomerMenu.DropDownItems[m].BackColor = backcolor2;
                }
                AddMenuService.BackColor = backcolor2;
                int num5 = AddMenuService.DropDownItems.Count - 1;
                for (int n = 0; n <= num5; n++)
                {
                    AddMenuService.DropDownItems[n].BackColor = backcolor2;
                }
                DelMenuService.BackColor = backcolor2;
                int num6 = DelMenuService.DropDownItems.Count - 1;
                for (int num7 = 0; num7 <= num6; num7++)
                {
                    DelMenuService.DropDownItems[num7].BackColor = backcolor2;
                }
                SearchMenuService.BackColor = backcolor2;
                int num8 = SearchMenuService.DropDownItems.Count - 1;
                for (int num9 = 0; num9 <= num8; num9++)
                {
                    SearchMenuService.DropDownItems[num9].BackColor = backcolor2;
                }
            }
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        private void Form1_Load(object sender, EventArgs e)
        {
            bool num = Process.GetProcessesByName(Process.GetCurrentProcess().ProcessName).Length > 1;
            string NomModule = "Parametres";
            string MotCle = "AdvancedMenu";
            string FichierIni = mFunction.ParsePath(Application.ExecutablePath, 7) + ".ini";
            checked
            {
                if (num & (Operators.CompareString(mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref FichierIni), "1", TextCompare: false) != 0))
                {
                    int num2 = 0;
                    string fileName = Process.GetCurrentProcess().MainModule.FileName;
                    string right = Process.GetCurrentProcess().ProcessName.ToUpper();
                    Process[] processes = Process.GetProcesses();
                    int num3 = Information.UBound(processes);
                    for (int i = 0; i <= num3; i++)
                    {
                        if (Operators.CompareString(processes[i].ProcessName.ToUpper(new CultureInfo("en-US", useUserOverride: false)), right, TextCompare: false) == 0)
                        {
                            string left = null;
                            try
                            {
                                left = processes[i].MainModule.FileName;
                            }
                            catch (Exception ex)
                            {
                                ProjectData.SetProjectError(ex);
                                Exception ex2 = ex;
                                ProjectData.ClearProjectError();
                            }
                            if (Operators.CompareString(left, fileName, TextCompare: false) == 0)
                            {
                                num2++;
                            }
                            if (num2 > 1 && Interaction.MsgBox("Application already open\r\n\r\nDo you really want to continue?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo | MsgBoxStyle.DefaultButton2, MyProject.Application.Info.Title) == MsgBoxResult.No)
                            {
                                Close();
                            }
                        }
                    }
                }
                BackColor = backcolor1;
                MenuStripFM.BackColor = backcolor1;
                StatusStripFM.BackColor = backcolor1;
                F2.BackColor = backcolor3;
                F3.BackColor = backcolor3;
                F4.BackColor = backcolor3;
                F5.BackColor = backcolor3;
                F6.BackColor = backcolor3;
                F9.BackColor = backcolor3;
                F10.BackColor = backcolor3;
                ButtonFiltreUpdates.BackColor = backcolor3;
                Label_version.BackColor = backcolor2;
                FiltrePatch.BackColor = backcolor2;
                ListSelectPatch.BackColor = backcolor2;
                ListSelectClient.BackColor = backcolor2;
                ListPatch.BackColor = backcolor2;
                ListAccesClient.BackColor = backcolor2;
                MyProject.Forms.CreateUpdate.BackColor = backcolor1;
                MyProject.Forms.CreateUpdate.TextPatchInformation.BackColor = backcolor2;
                MyProject.Forms.CreateUpdate.ComboBoxPatchName.BackColor = backcolor2;
                MyProject.Forms.CreateUpdate.ButtonExit.BackColor = backcolor3;
                MyProject.Forms.CreateUpdate.ButtonCreating.BackColor = backcolor3;
                MyProject.Forms.CreateUpdate.StatusStrip1.BackColor = backcolor1;
                MyProject.Forms.CreateUpdate.ButtonGetData.BackColor = backcolor3;
                MyProject.Forms.CreateUpdate.ButtonGetVersion.BackColor = backcolor3;
                MyProject.Forms.CreateUpdate.ButtonGetTarifs.BackColor = backcolor3;
                MyProject.Forms.CreateUpdate.ButtonPigeAuto.BackColor = backcolor3;
                MyProject.Forms.DataRecoveryP.BackColor = backcolor1;
                MyProject.Forms.DataRecoveryP.ButtonExit.BackColor = backcolor3;
                MyProject.Forms.DataRecoveryP.ButtonCreating.BackColor = backcolor3;
                MyProject.Forms.DataRecoveryP.ComboBox1.BackColor = backcolor2;
                MyProject.Forms.DataRecoveryP.TreeViewList.BackColor = backcolor2;
                MyProject.Forms.DataRecoveryP.TreeViewSelected.BackColor = backcolor2;
                MyProject.Forms.DataRecoveryP.ToolStripProgressBar1.BackColor = backcolor2;
                MyProject.Forms.DataRecoveryP.StatusStrip1.BackColor = backcolor1;
                MyProject.Forms.DataRecovery.BackColor = backcolor1;
                MyProject.Forms.DataRecovery.ButtonExit.BackColor = backcolor3;
                MyProject.Forms.DataRecovery.ButtonCreating.BackColor = backcolor3;
                MyProject.Forms.DataRecovery.ButtonZip.BackColor = backcolor3;
                MyProject.Forms.DataRecovery.ButtonUnZip.BackColor = backcolor3;
                MyProject.Forms.DataRecovery.ButtonParcourir.BackColor = backcolor3;
                MyProject.Forms.DataRecovery.TreeViewSelected.BackColor = backcolor2;
                MyProject.Forms.DataRecovery.TreeViewList.BackColor = backcolor2;
                MyProject.Forms.DataRecovery.ToolStripProgressBar2.BackColor = backcolor2;
                MyProject.Forms.DataRecovery.ComboBox1.BackColor = backcolor2;
                MyProject.Forms.DataRecovery.StatusStrip1.BackColor = backcolor1;
                MyProject.Forms.PleaseWait.BackColor = backcolor1;
                MyProject.Forms.GetElements.BackColor = backcolor1;
                MyProject.Forms.GetElements.ComboBoxVersion.BackColor = backcolor2;
                MyProject.Forms.GetElements.ButtonIgnore.BackColor = backcolor3;
                MyProject.Forms.GetElements.ButtonCancel.BackColor = backcolor3;
                MyProject.Forms.GetElements.ButtonGet.BackColor = backcolor3;
                string text = mFunction.ParsePath(Application.ExecutablePath, 7);
                FacilityManagementIni = text + ".ini";
                if (!File.Exists(FacilityManagementIni))
                {
                    Interaction.MsgBox("Missing configuration file", MsgBoxStyle.Critical, MyProject.Application.Info.Title);
                    ProjectData.EndApp();
                }
                ToolStripStatusLabelDisk.Text = "";
                F2.Text = "F2 new client";
                F3.Text = "F3 create patch";
                F4.Text = "F4 edit client";
                F5.Text = "F5 refresh";
                F6.Text = "F6 add/send patch";
                F9.Text = "F9 Search customer";
                F10.Text = "F10 Search database";
                //Label_version.Text = "v" + Application.ProductVersion.ToString();
                Label_version.Text = "v" + " toto";
                if (Operators.CompareString(Interaction.Command(), "ReStart", TextCompare: false) == 0)
                {
                    HttpSendMessage();
                }
                double num4 = MyProject.Computer.FileSystem.GetDriveInfo(text).AvailableFreeSpace;
                int result = 0;
                FichierIni = "Parametres";
                MotCle = "FreeSpace";
                if (!int.TryParse(mFileIni.Select_GetIniString(ref FichierIni, ref MotCle, ref FacilityManagementIni).Trim(), out result))
                {
                    result = 500;
                    mFileIni.Select_WriteIniString("Parametres", "FreeSpace", Conversions.ToString(result), FacilityManagementIni);
                }
                if ((double)result * 1048576.0 > num4)
                {
                    Interaction.MsgBox("Insufficient disk space. (Free space: " + mFunction.ConvertOctets(num4) + ")", MsgBoxStyle.Critical, MyProject.Application.Info.Title);
                    bInsufficientDiskSpace = true;
                }
                ///WARNING
                //else if (mConnection.MConnection_GetLastManage())
                //{
                //    Process.Start(Application.ExecutablePath, "ReStart");
                //    Close();
                //}
                MyProject.Forms.PleaseWait.Show();
                Application.DoEvents();
                MAP_DRIVE mAP_DRIVE = default(MAP_DRIVE);
                int num5 = 0;
                while (true)
                {
                    MotCle = "Parametres";
                    FichierIni = "MapDrive" + Conversions.ToString(num5).Trim();
                    if (!mAP_DRIVE.Value(mFileIni.Select_GetIniString(ref MotCle, ref FichierIni, ref FacilityManagementIni)))
                    {
                        break;
                    }
                    if (!File.Exists(mAP_DRIVE.TEST))
                    {
                        mMapDrive.UnMapDrive(mAP_DRIVE.LETTER);
                        mMapDrive.MapDrive(mAP_DRIVE.LETTER, mAP_DRIVE.PATH);
                    }
                    num5++;
                }
                FichierIni = "Parametres";
                MotCle = "Opacity";
                nvOpacity = Conversions.ToDouble(mFileIni.Select_GetIniString(ref FichierIni, ref MotCle, ref FacilityManagementIni).Trim());
                if (nvOpacity == 0.0)
                {
                    nvOpacity = 100.0;
                }
                base.Opacity = nvOpacity / 100.0;
                MotCle = "Parametres";
                FichierIni = "ISPROJECTDIR";
                ISPROJECTDIR = mFileIni.Select_GetIniString(ref MotCle, ref FichierIni, ref FacilityManagementIni).Trim();
                FichierIni = "Parametres";
                MotCle = "DATAFRET";
                DATAFRET = mFileIni.Select_GetIniString(ref FichierIni, ref MotCle, ref FacilityManagementIni).Trim();
                MotCle = "Parametres";
                FichierIni = "DATA";
                DATA = mFileIni.Select_GetIniString(ref MotCle, ref FichierIni, ref FacilityManagementIni).Trim();
                if (bInsufficientDiskSpace)
                {
                    Interaction.MsgBox("Probleme d'espace disque !\r\n\r\nLa création de mise à jour sera impossible.\r\n\r\nVeuillez contacter le responsable.", MsgBoxStyle.Critical, MyProject.Application.Info.Title);
                    svAppBuild = null;
                }
                else
                {
                    FichierIni = "Parametres";
                    MotCle = "InstallShield";
                    svAppBuild = mFileIni.Select_GetIniString(ref FichierIni, ref MotCle, ref FacilityManagementIni).Trim();
                }
                if (File.Exists(svAppBuild))
                {
                    MotCle = "Parametres";
                    FichierIni = "LicenseInstallShield";
                    string obj = mFileIni.Select_GetIniString(ref MotCle, ref FichierIni, ref FacilityManagementIni);
                    string value = MacAddress.GetValue(obj);
                    if (Strings.StrComp(obj, value, CompareMethod.Text) != 0)
                    {
                        Interaction.MsgBox("Probleme de licence Installshield !\r\n\r\nLa création de mise à jour sera impossible.\r\n\r\nVeuillez contacter le responsable.", MsgBoxStyle.Critical, MyProject.Application.Info.Title + " : " + value);
                        svAppBuild = null;
                    }
                }
                else
                {
                    svAppBuild = null;
                }
                FichierIni = "Parametres";
                MotCle = "UseShellExecute";
                bUseShellExecute = Operators.CompareString(mFileIni.Select_GetIniString(ref FichierIni, ref MotCle, ref FacilityManagementIni), "1", TextCompare: false) == 0;
                MyProject.User.Name.ToLower();
                _ = new string[2] { "pc-olivier\\olivier", "kt\\olivier.marsot" };
                bAdvancedMenu = true;
                MotCle = "SQL";
                FichierIni = "Server";
                SQL_Serveur = mFileIni.Select_GetIniString(ref MotCle, ref FichierIni, ref FacilityManagementIni).Trim();
                FichierIni = "SQL";
                MotCle = "Login";
                SQL_Login = mFileIni.Select_GetIniString(ref FichierIni, ref MotCle, ref FacilityManagementIni).Trim();
                MotCle = "SQL";
                FichierIni = "Password";
                SQL_Password = mFileIni.Select_GetIniString(ref MotCle, ref FichierIni, ref FacilityManagementIni).Trim();
                FichierIni = "SQL";
                MotCle = "NoErrorMessage";
                SQL_NoErrorMessage = Operators.CompareString(mFileIni.Select_GetIniString(ref FichierIni, ref MotCle, ref FacilityManagementIni).Trim(), "0", TextCompare: false) != 0;
                SQL_SiteInternet_Serveur = SQL_Serveur;
                InitMenu();
                InitImage();
                MotCle = "Parametres";
                FichierIni = "SoftwareDefault";
                MenuSoft = mFileIni.Select_GetIniString(ref MotCle, ref FichierIni, ref FacilityManagementIni).Trim();
                FichierIni = "Parametres";
                MotCle = "CountryDefault";
                MenuPays = mFileIni.Select_GetIniString(ref FichierIni, ref MotCle, ref FacilityManagementIni).Trim();
                if (Operators.CompareString(MenuSoft, "", TextCompare: false) != 0)
                {
                    Init_Software();
                }
                if (Operators.CompareString(MenuPays, "", TextCompare: false) != 0)
                {
                    Init_Country();
                }
                GetCustomerOfCountry();
                MyProject.Forms.PleaseWait.Hide();
                base.KeyPreview = true;
                Application.DoEvents();
            }
        }

        private void ListPatch_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            mControlFormBox.SelectedPatch();
        }

        private void ListSelectClient_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            mControlFormBox.UnSelectedClient();
        }

        private void ListAccesClient_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            mControlFormBox.SelectedClient();
        }

        private void ButtonUnAddAllClients_Click(object sender, EventArgs e)
        {
            mControlFormBox.UnSelectedAllClient();
        }

        private void ButtonAddAllClients_Click(object sender, EventArgs e)
        {
            mControlFormBox.SelectedAllClient();
        }

        private void ButtonUnAddAllPatchs_Click(object sender, EventArgs e)
        {
            mControlFormBox.UnSelectedAllPatch();
        }

        private void ExitMenu_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void ListSelectPatch_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            mControlFormBox.UnSelectedPatch();
        }

        private void CustomerMenu_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            Disabled_FormBox();
            string key = e.ClickedItem.Text;
            int num = cMSDic[key];
            CustomerMenu.HideDropDown();
            switch (num)
            {
                case 9:
                    AddPatchToClient();
                    break;
                case 43:
                    AddPatchToClient(null, null, MsgFinishVisible: false);
                    if (Interaction.MsgBox("Do you want to send the patches to the FTP server?", MsgBoxStyle.YesNo | MsgBoxStyle.Information | MsgBoxStyle.DefaultButton2, "Facility Management - Transfer FTP Customers and patchs") == MsgBoxResult.Yes)
                    {
                        Send2FTP(null, null, MsgFinishVisible: false);
                    }
                    Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal);
                    break;
                case 13:
                    DelPatchToClient();
                    break;
                case 31:
                    DelAllPatchBeforeToClient();
                    break;
                case 32:
                    Reindexing();
                    break;
            }
            Enabled_FormBox();
        }

        private void UpdateMenu_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            string key = e.ClickedItem.Text;
            switch (cMSDic[key])
            {
                case 10:
                    Send2FTP();
                    break;
                case 44:
                    CRC2File();
                    break;
                case 49:
                    SkipPatchToClient();
                    break;
                case 50:
                    NotSkipPatchToClient();
                    break;
                case 52:
                    AdmPatchToClient();
                    break;
                case 53:
                    NotAdmPatchToClient();
                    break;
                case 65:
                    ChargePatchFromFile();
                    break;
            }
        }

        private void AddMenuService_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            string key = e.ClickedItem.Text;
            int num = cMSDic[key];
            bool flag = Strings.StrComp(MenuSoft, "Medial 2", CompareMethod.Text) == 0;
            switch (num)
            {
                case 4:
                    if (flag)
                    {
                        Add2Customer("%REQUESTINFO%", null, MsgFinishVisible: false);
                        Add2Customer("%SYSTEMINFO%", null, MsgFinishVisible: false);
                        Add2Customer("%CONFIGURATIONXML%", null, MsgFinishVisible: false);
                        Add2Customer("%MEDIALSUPERVISORINI%", null, MsgFinishVisible: false);
                        Add2Customer("%SERVICESSTATUS%", null, MsgFinishVisible: false);
                    }
                    else
                    {
                        Add2Customer("%PING%", null, MsgFinishVisible: false);
                        Add2Customer("%FORCEWAIT%", null, MsgFinishVisible: false);
                        Add2Customer("%FORCEID%", null, MsgFinishVisible: false);
                    }
                    Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal);
                    break;
                case 22:
                    Add2Customer("%REQUESTINFO%");
                    break;
                case 23:
                    if (flag)
                    {
                        Add2Customer("%KILLPROCESS%KM.Medial.Services");
                    }
                    else
                    {
                        Add2Customer("%KILLPROCESS%");
                    }
                    break;
                case 24:
                    Add2Customer("%ZIPLOGS%");
                    break;
                case 26:
                    Add2Customer("%SYSTEMINFO%");
                    break;
                case 27:
                    Add2Customer("%CONFIGURATIONXML%");
                    break;
                case 28:
                    Add2Customer("%MEDIALSUPERVISORINI%");
                    break;
                case 29:
                    Add2Customer("%SERVICESSTATUS%");
                    break;
                case 30:
                    Add2Customer("%PING%");
                    break;
                case 36:
                    Add2Customer("%EXPORTUFR%");
                    break;
                case 37:
                    Add2Customer("%NOEXPORTUFR%");
                    break;
                case 38:
                    Add2Customer("%FORCEWAIT%");
                    break;
                case 39:
                    Add2Customer("%FORCEID%");
                    break;
                case 51:
                    Add2Customer("%PROXY%");
                    break;
            }
        }

        private void SearchMenuService_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
        }

        private void DelMenuService_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
        }

        private void ToolsMenu_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            Disabled_FormBox();
            string key = e.ClickedItem.Text;
            int num = cMSDic[key];
            ToolsMenu.HideDropDown();
            checked
            {
                switch (num)
                {
                    case 11:
                        {
                            ref string menuSoft4 = ref MenuSoft;
                            string MotCle = "DataRecovery";
                            if (Operators.CompareString(mFileIni.Select_GetIniString(ref menuSoft4, ref MotCle, ref FacilityManagementIni).Trim(), "P", TextCompare: false) == 0)
                            {
                                MyProject.Forms.DataRecoveryP.ShowDialog();
                                break;
                            }
                            ref string menuSoft5 = ref MenuSoft;
                            MotCle = "DataRecovery";
                            if (Operators.CompareString(mFileIni.Select_GetIniString(ref menuSoft5, ref MotCle, ref FacilityManagementIni).Trim(), "I", TextCompare: false) == 0)
                            {
                                MyProject.Forms.DataRecovery.ShowDialog();
                            }
                            break;
                        }
                    case 12:
                        if (DateAndTime.DateDiff(DateInterval.Day, DIndex, DateTime.Now) > 0)
                        {
                            /// WARNING
                            //_ = int.MaxValue * 2;
                        }
                        MyProject.Forms.CreateUpdate.ShowDialog();
                        break;
                    case 15:
                        if (Interaction.MsgBox("Would you erase data from all countries", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo, "Clear Data") == MsgBoxResult.Yes)
                        {
                            ref string menuSoft6 = ref MenuSoft;
                            string MotCle = "Destination";
                            string[] directories = Directory.GetDirectories(mFileIni.Select_GetIniString(ref menuSoft6, ref MotCle, ref FacilityManagementIni).Trim());
                            for (int j = 0; j < directories.Length; j++)
                            {
                                Directory.Delete(directories[j], recursive: true);
                            }
                            Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal);
                        }
                        break;
                    case 18:
                        {
                            if (Interaction.MsgBox("Are you sure you want to send patches automatically (Pige)?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo | MsgBoxStyle.DefaultButton2, "Send Automatic") != MsgBoxResult.Yes)
                            {
                                break;
                            }
                            mControlFormBox.UnSelectedAllClient();
                            mControlFormBox.UnSelectedAllPatch();
                            MyProject.Forms.GetElements.ComboBoxVersion.Items.Clear();
                            MyProject.Forms.GetElements.ComboBoxVersion.Text = "";
                            MyProject.Forms.GetElements.Text = "Send Automatic";
                            MyProject.Forms.GetElements.ChooseLabel.Text = "Select a patch to send:";
                            MyProject.Forms.GetElements.ButtonGet.Text = "Send";
                            Icon icon = Icon.FromHandle(Resources.ftp.GetHicon());
                            MyProject.Forms.GetElements.Icon = icon;
                            ref string menuSoft = ref MenuSoft;
                            string MotCle = "PatchFind";
                            string searchPattern = mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref FacilityManagementIni).Trim();
                            string[] files = Directory.GetFiles(mPathExes, searchPattern);
                            files = (from w in files
                                     select (w) into w
                                     orderby File.GetCreationTime(w) descending
                                     select w).ToArray();
                            files = files.Select((string w) => Strings.Replace(w, mPathExes, "")).ToArray();
                            files = files.Select((string w) => Strings.Replace(w, "All.exe", "")).ToArray();
                            MyProject.Forms.GetElements.ComboBoxVersion.Items.AddRange(files);
                            if (MyProject.Forms.GetElements.ComboBoxVersion.Items.Count > 0)
                            {
                                MyProject.Forms.GetElements.ComboBoxVersion.Text = Conversions.ToString(MyProject.Forms.GetElements.ComboBoxVersion.Items[0]);
                            }
                            if (MyProject.Forms.GetElements.ShowDialog() != DialogResult.OK)
                            {
                                break;
                            }
                            string text = MyProject.Forms.GetElements.ComboBoxVersion.Text;
                            int num2 = Array.IndexOf(files.ToArray(), text);
                            string text2 = null;
                            if (files.Length - 1 > num2)
                            {
                                text2 = files[num2 + 1];
                            }
                            ref string menuSoft2 = ref MenuSoft;
                            MotCle = "Key1";
                            string text3 = mFileIni.Select_GetIniString(ref menuSoft2, ref MotCle, ref FacilityManagementIni).Trim();
                            int num3 = 1;
                            while (Operators.CompareString(text3, "", TextCompare: false) != 0)
                            {
                                string[] array = Strings.Split(text3, ",");
                                mControlFormBox.SelectedPatch(text + array[1] + mExtFileExes);
                                for (int i = 2; i < array.Length; i++)
                                {
                                    if (AliasDic.ContainsKey(array[i]))
                                    {
                                        string text4 = AliasDic[array[i]];
                                        if (File.Exists(mPathAcces + text4 + mExtFileAcces))
                                        {
                                            mControlFormBox.SelectedClient(array[i]);
                                        }
                                    }
                                    else if (File.Exists(mPathAcces + array[i] + mExtFileAcces))
                                    {
                                        mControlFormBox.SelectedClient(array[i]);
                                    }
                                    Application.DoEvents();
                                }
                                if (ListSelectClient.Items.Count > 0)
                                {
                                    AddPatchToClient(null, null, false, text2 + array[1]);
                                    Send2FTP(null, null, MsgFinishVisible: false);
                                }
                                mControlFormBox.UnSelectedAllClient();
                                mControlFormBox.UnSelectedAllPatch();
                                Application.DoEvents();
                                num3++;
                                ref string menuSoft3 = ref MenuSoft;
                                MotCle = "Key" + Strings.Trim(Conversions.ToString(num3));
                                text3 = mFileIni.Select_GetIniString(ref menuSoft3, ref MotCle, ref FacilityManagementIni).Trim();
                            }
                            Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal);
                            break;
                        }
                    case 20:
                        SelectedAllClientPige();
                        break;
                    case 19:
                        Init_Software();
                        GetCustomerOfCountry();
                        break;
                }
                Enabled_FormBox();
            }
        }

        private void CountryMenu_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            MenuPays = e.ClickedItem.Text;
            mFileIni.Select_WriteIniString("Parametres", "CountryDefault", MenuPays, FacilityManagementIni);
            Init_Country();
            GetCustomerOfCountry();
        }

        private void Init_Country()
        {
            if (Operators.CompareString(mPathExes, "", TextCompare: false) != 0)
            {
                mPathAcces = mPathExes + CountryDic[MenuPays];
            }
            else
            {
                mPathAcces = "";
            }
            if (Operators.CompareString(MenuSoft, "", TextCompare: false) != 0)
            {
                Text = "Facility Management - " + MenuSoft + " - " + MenuPays;
            }
            else
            {
                Text = "Facility Management - " + MenuPays;
            }
            CountryMenu.Image = DicPaysImage[MenuPays];
            CountryMenu.Text = MenuPays;
        }

        private void SelectedAllClientPige()
        {
            mControlFormBox.UnSelectedAllClient();
            mControlFormBox.UnSelectedAllPatch();
            ref string menuSoft = ref MenuSoft;
            string MotCle = "Key1";
            string text = mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref FacilityManagementIni).Trim();
            int num = 1;
            checked
            {
                while (Operators.CompareString(text, "", TextCompare: false) != 0)
                {
                    string[] array = Strings.Split(text, ",");
                    for (int i = 2; i < array.Length; i++)
                    {
                        if (AliasDic.ContainsKey(array[i]))
                        {
                            string text2 = AliasDic[array[i]];
                            if (File.Exists(mPathAcces + text2 + mExtFileAcces))
                            {
                                mControlFormBox.SelectedClient(array[i]);
                            }
                        }
                        else if (File.Exists(mPathAcces + array[i] + mExtFileAcces))
                        {
                            mControlFormBox.SelectedClient(array[i]);
                        }
                        Application.DoEvents();
                    }
                    Application.DoEvents();
                    num++;
                    ref string menuSoft2 = ref MenuSoft;
                    MotCle = "Key" + Strings.Trim(Conversions.ToString(num));
                    text = mFileIni.Select_GetIniString(ref menuSoft2, ref MotCle, ref FacilityManagementIni).Trim();
                }
            }
        }

        private void Init_Software()
        {
            if (!SoftwareDic.ContainsKey(MenuSoft))
            {
                return;
            }
            mPathExes = SoftwareDic[MenuSoft];
            if (Operators.CompareString(MenuPays, "", TextCompare: false) != 0)
            {
                Text = "Facility Management - " + MenuSoft + " - " + MenuPays;
                mPathAcces = mPathExes + CountryDic[MenuPays];
            }
            else
            {
                Text = "Facility Management - " + MenuSoft;
                mPathAcces = "";
            }
            CreateListExes();
            if (Operators.CompareString(FiltrePatch.Text, null, TextCompare: false) != 0)
            {
                PatchFilter();
            }
            CountryMenu.DropDownItems.Clear();
            checked
            {
                foreach (string key in CountryDic.Keys)
                {
                    if (Directory.Exists(mPathExes + CountryDic[key]))
                    {
                        CountryMenu.DropDownItems.Add(key, DicPaysImage[key]);
                        CountryMenu.DropDownItems[CountryMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                        CountryMenu.DropDownItems[CountryMenu.DropDownItems.Count - 1].BackColor = backcolor2;
                    }
                }
                if (CountryMenu.DropDownItems.Count == 0)
                {
                    CountryMenu.Image = Resources.avertissement;
                }
                else if (CountryMenu.DropDownItems.Count == 1)
                {
                    MenuPays = CountryMenu.DropDownItems[0].Text;
                    CountryMenu.DropDownItems.Clear();
                    CountryMenu.Image = DicPaysImage[MenuPays];
                    CountryMenu.Text = MenuPays;
                    Text = "Facility Management - " + MenuSoft + " - " + MenuPays;
                    mPathAcces = mPathExes + CountryDic[MenuPays];
                }
                SoftwareMenu.Image = DicSoftImage[MenuSoft];
                SoftwareMenu.Text = MenuSoft;
                ToolStripItem toolStripItem = ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(12))];
                ref string menuSoft = ref MenuSoft;
                string MotCle = "CreatePatch";
                toolStripItem.Visible = Operators.CompareString(mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref FacilityManagementIni).Trim(), "0", TextCompare: false) != 0;
                ToolStripItem toolStripItem2 = ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(11))];
                ref string menuSoft2 = ref MenuSoft;
                MotCle = "ToolsMenuGetData";
                toolStripItem2.Visible = Conversion.Val(mFileIni.Select_GetIniString(ref menuSoft2, ref MotCle, ref FacilityManagementIni).Trim()) == 1.0;
                ToolStripItem toolStripItem3 = ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(18))];
                ref string menuSoft3 = ref MenuSoft;
                MotCle = "ToolsMenuPige";
                toolStripItem3.Visible = Conversion.Val(mFileIni.Select_GetIniString(ref menuSoft3, ref MotCle, ref FacilityManagementIni).Trim()) == 1.0;
                ToolStripItem toolStripItem4 = ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(15))];
                ref string menuSoft4 = ref MenuSoft;
                MotCle = "ToolsMenuEraseData";
                toolStripItem4.Visible = Conversion.Val(mFileIni.Select_GetIniString(ref menuSoft4, ref MotCle, ref FacilityManagementIni).Trim()) == 1.0;
                ToolStripItem toolStripItem5 = ToolsMenu.DropDownItems[ToolsMenu.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(20))];
                ref string menuSoft5 = ref MenuSoft;
                MotCle = "ToolsMenuPige";
                toolStripItem5.Visible = Conversion.Val(mFileIni.Select_GetIniString(ref menuSoft5, ref MotCle, ref FacilityManagementIni).Trim()) == 1.0;
                CustomerMenu.DropDownItems[CustomerMenu.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(25))].Visible = bAdvancedMenu;
                bool flag = Strings.StrComp(MenuSoft, "Medial 2", CompareMethod.Text) == 0;
                bool visible = Strings.StrComp(MenuSoft, "Atelier Radio", CompareMethod.Text) == 0;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(22))].Visible = flag;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(24))].Visible = flag;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(26))].Visible = flag;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(27))].Visible = flag;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(28))].Visible = flag;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(29))].Visible = flag;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(36))].Visible = visible;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(37))].Visible = visible;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(38))].Visible = visible;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(39))].Visible = visible;
                AddMenuService.DropDownItems[AddMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(51))].Visible = !flag;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(22))].Visible = flag;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(24))].Visible = flag;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(26))].Visible = flag;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(27))].Visible = flag;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(28))].Visible = flag;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(29))].Visible = flag;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(36))].Visible = visible;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(37))].Visible = visible;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(38))].Visible = visible;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(39))].Visible = visible;
                DelMenuService.DropDownItems[DelMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(51))].Visible = !flag;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(22))].Visible = flag;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(24))].Visible = flag;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(26))].Visible = flag;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(27))].Visible = flag;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(28))].Visible = flag;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(29))].Visible = flag;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(36))].Visible = visible;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(37))].Visible = visible;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(38))].Visible = visible;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(39))].Visible = visible;
                SearchMenuService.DropDownItems[SearchMenuService.DropDownItems.IndexOfKey(cMSDic.Keys.ElementAtOrDefault(51))].Visible = !flag;
                ref string menuSoft6 = ref MenuSoft;
                MotCle = "UpdateApps";
                bool flag2 = Conversion.Val(mFileIni.Select_GetIniString(ref menuSoft6, ref MotCle, ref FacilityManagementIni).Trim()) != 1.0;
                ListAccesClient.Visible = flag2;
                ListSelectClient.Visible = flag2;
                CustomerMenu.Enabled = flag2;
                CountryMenu.Enabled = flag2;
                Label2.Visible = flag2;
                ListSelectClient.Visible = flag2;
                Label1.Visible = flag2;
                ListAccesClient.Visible = flag2;
                ToolsMenu.Enabled = flag2;
                mConnection.MConnection_initFTP(ref MenuSoft);
                Application.DoEvents();
            }
        }

        private void SoftwareMenu_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            MyProject.Forms.PleaseWait.Show();
            Application.DoEvents();
            MenuSoft = e.ClickedItem.Text;
            mFileIni.Select_WriteIniString("Parametres", "SoftwareDefault", MenuSoft, FacilityManagementIni);
            Init_Software();
            GetCustomerOfCountry();
            MyProject.Forms.PleaseWait.Hide();
            Application.DoEvents();
        }

        private void CMS_ListAccesClient_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            Disabled_FormBox();
            if (Operators.CompareString(mPathAcces, null, TextCompare: false) != 0)
            {
                string key = e.ClickedItem.Text;
                int num = cMSDic[key];
                string svClient = Conversions.ToString(ListAccesClient.SelectedItem);
                if (Operators.CompareString(svClient, null, TextCompare: false) != 0)
                {
                    svClient = AliasDic[svClient];
                }
                CMS_ListAccesClient.Hide();
                switch (num)
                {
                    case 0:
                        {
                            string text3 = Interaction.InputBox("New Client:", "", "", CMS_ListAccesClient.Bounds.X, CMS_ListAccesClient.Bounds.Y).Trim();
                            if (Operators.CompareString(text3, null, TextCompare: false) == 0)
                            {
                                break;
                            }
                            text3.Trim();
                            AliasCompany(text3);
                            foreach (KeyValuePair<string, string> item in CountryDic)
                            {
                                if (File.Exists(mPathExes + item.Value + text3 + mExtFileAcces))
                                {
                                    Interaction.MsgBox("The client '" + text3 + "' already exists in'" + item.Key + "'.", MsgBoxStyle.Exclamation, "Facility Management - New Client");
                                    Enabled_FormBox();
                                    return;
                                }
                            }
                            string path = mPathAcces + text3 + mExtFileAcces;
                            if (File.Exists(path))
                            {
                                File.Delete(path);
                            }
                            File.WriteAllText(path, AddCRC2String(BaliseAccesEndOfFile), Encoding.Default);
                            ListAccesClient.Items.Add(text3);
                            break;
                        }
                    case 8:
                        {
                            string text4 = Interaction.InputBox("New Client:", "", "", CMS_ListAccesClient.Bounds.X, CMS_ListAccesClient.Bounds.Y);
                            if (Operators.CompareString(text4, null, TextCompare: false) == 0)
                            {
                                break;
                            }
                            text4.Trim();
                            AliasCompany(text4);
                            foreach (KeyValuePair<string, string> item2 in CountryDic)
                            {
                                if (File.Exists(mPathExes + item2.Value + text4 + mExtFileAcces))
                                {
                                    Interaction.MsgBox("The client '" + text4 + "' already exists in'" + item2.Key + "'.", MsgBoxStyle.Exclamation, "Facility Management - New Client");
                                    Enabled_FormBox();
                                    return;
                                }
                            }
                            string sourceFileName = mPathAcces + svClient + mExtFileAcces;
                            string text5 = mPathAcces + text4 + mExtFileAcces;
                            if (File.Exists(text5))
                            {
                                File.Delete(text5);
                            }
                            File.Copy(sourceFileName, text5);
                            DateTime now = DateAndTime.Now;
                            File.SetCreationTime(text5, now);
                            File.SetLastAccessTime(text5, now);
                            File.SetLastWriteTime(text5, now);
                            ListAccesClient.Items.Add(text4);
                            break;
                        }
                    case 1:
                        if (Operators.CompareString(svClient, null, TextCompare: false) != 0)
                        {
                            string svApplication = Environment.GetEnvironmentVariable("windir") + "\\notepad.exe";
                            string svCommandLine = mPathAcces + svClient + mExtFileAcces;
                            ProcessWindowStyle Style = ProcessWindowStyle.Normal;
                            byte WaitInfinity = 0;
                            mLaunchAppAndWait.LaunchAppAndWait(svApplication, svCommandLine,  Style,  WaitInfinity);
                        }
                        break;
                    case 2:
                        if (Operators.CompareString(svClient, null, TextCompare: false) != 0 && Interaction.MsgBox("Are you sure you want to remove the client (" + svClient + ") ?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo | MsgBoxStyle.DefaultButton2) == MsgBoxResult.Yes && Interaction.MsgBox("Are you sure you want to remove the client (" + svClient + ") ?", MsgBoxStyle.YesNo | MsgBoxStyle.Critical | MsgBoxStyle.DefaultButton2) == MsgBoxResult.Yes)
                        {
                            AliasDic.Remove(Conversions.ToString(ListAccesClient.SelectedItem));
                            string path2 = mPathAcces + svClient + mExtFileAcces;
                            if (File.Exists(path2))
                            {
                                File.Delete(path2);
                            }
                            mConnection.MConnection_DeleteFile(svClient + "." + mCheckSum.AppendString(mEasyCrypt.Crypter(svClient, "sha1")));
                            ListAccesClient.Items.RemoveAt(ListAccesClient.SelectedIndex);
                        }
                        break;
                    case 3:
                        if (Operators.CompareString(svClient, null, TextCompare: false) != 0)
                        {
                            mControlFormBox.SelectedClient();
                        }
                        break;
                    case 4:
                        if (Operators.CompareString(svClient, null, TextCompare: false) != 0)
                        {
                            mControlFormBox.SelectedAllClient();
                        }
                        break;
                    case 14:
                        if (Operators.CompareString(svClient, null, TextCompare: false) != 0)
                        {
                            mControlFormBox.UnSelectedAllClient();
                            mControlFormBox.SelectedClient();
                            AddAllUpdate2Client(ref svClient);
                        }
                        break;
                    case 20:
                        SelectedAllClientPige();
                        break;
                    case 21:
                        InvertClient();
                        break;
                    case 33:
                        AddGroup();
                        break;
                    case 40:
                        CreateSocieteAuto(svClient);
                        break;
                    case 48:
                        {
                            if (Operators.CompareString(svClient, null, TextCompare: false) == 0)
                            {
                                break;
                            }
                            string text = Conversions.ToString(ListAccesClient.SelectedItem);
                            string text2 = Interaction.InputBox("Rename Client:", "", text, CMS_ListAccesClient.Bounds.X, CMS_ListAccesClient.Bounds.Y).Trim();
                            if (Operators.CompareString(text2, null, TextCompare: false) == 0)
                            {
                                break;
                            }
                            text2.Trim();
                            foreach (string key2 in AliasDic.Keys)
                            {
                                if (Operators.CompareString(text2, key2, TextCompare: false) == 0)
                                {
                                    Interaction.MsgBox("The client '" + text2 + "' already exists.", MsgBoxStyle.Exclamation, "Facility Management - Rename Client");
                                    Enabled_FormBox();
                                    return;
                                }
                            }
                            foreach (string value in AliasDic.Values)
                            {
                                if (Operators.CompareString(text2, value, TextCompare: false) == 0)
                                {
                                    Interaction.MsgBox("The Alias '" + text2 + "' already exists.", MsgBoxStyle.Exclamation, "Facility Management - Rename Client");
                                    Enabled_FormBox();
                                    return;
                                }
                            }
                            AliasDic.Remove(text);
                            ListAccesClient.Items.RemoveAt(ListAccesClient.SelectedIndex);
                            AliasDic.Add(text2, svClient);
                            ListAccesClient.Items.Add(text2);
                            mFileIni.Select_WriteIniString("Alias", svClient, text2, FacilityManagementIni);
                            break;
                        }
                }
            }
            Enabled_FormBox();
        }

        private void CMS_ListPatch_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            Disabled_FormBox();
            string key = e.ClickedItem.Text;
            int num = cMSDic[key];
            string text = Conversions.ToString(ListPatch.SelectedItem);
            CMS_ListPatch.Hide();
            checked
            {
                switch (num)
                {
                    case 0:
                        if (DateAndTime.DateDiff(DateInterval.Day, DIndex, DateTime.Now) > 0)
                        {

                            ///WARNING
                            //_ = int.MaxValue * 2;
                        }
                        MyProject.Forms.CreateUpdate.ShowDialog();
                        break;
                    case 1:
                        if (Operators.CompareString(text, null, TextCompare: false) != 0)
                        {
                            string svApplication = Environment.GetEnvironmentVariable("windir") + "\\notepad.exe";
                            string svCommandLine = mPathExes + mFunction.ParsePath(text, 5) + mExtFileInfo;
                            ProcessWindowStyle Style = ProcessWindowStyle.Normal;
                            byte WaitInfinity = 0;
                            mLaunchAppAndWait.LaunchAppAndWait(svApplication, svCommandLine,  Style,  WaitInfinity);
                        }
                        break;
                    case 2:
                        if (Operators.CompareString(text, null, TextCompare: false) != 0 && Interaction.MsgBox("Are you sure you want to remove the path (" + text + ") ?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo | MsgBoxStyle.DefaultButton2) == MsgBoxResult.Yes && Interaction.MsgBox("Are you sure you want to remove the path (" + text + ") ?", MsgBoxStyle.YesNo | MsgBoxStyle.Critical | MsgBoxStyle.DefaultButton2) == MsgBoxResult.Yes)
                        {
                            string path = mPathExes + mFunction.ParsePath(text, 5) + mExtFileInfo;
                            if (File.Exists(path))
                            {
                                File.Delete(path);
                            }
                            string path2 = mPathExes + mFunction.ParsePath(text, 5) + mExtFileCRC;
                            if (File.Exists(path2))
                            {
                                File.Delete(path2);
                            }
                            path = mPathExes + text;
                            if (File.Exists(path))
                            {
                                File.Delete(path);
                            }
                            DicExes.Remove(text);
                            ListPatch.Items.RemoveAt(ListPatch.SelectedIndex);
                        }
                        break;
                    case 3:
                        if (Operators.CompareString(text, null, TextCompare: false) != 0)
                        {
                            mControlFormBox.SelectedPatch();
                        }
                        break;
                    case 19:
                        CreateListExes();
                        PatchFilter();
                        break;
                }
                Label3.Text = "Selected Updates (" + Conversions.ToString(ListSelectPatch.Items.Count) + "):";
                Label4.Text = "Updates List (" + Conversions.ToString(ListPatch.Items.Count) + "):";
                Enabled_FormBox();
            }
        }

        private void AddAllUpdate2Client(ref string svClient)
        {
            mControlFormBox.UnSelectedAllPatch();
            string[] array = File.ReadAllLines(mPathAcces + svClient + mExtFileAcces, Encoding.Default);
            foreach (string text in array)
            {
                if (Operators.CompareString(text, BaliseAccesEndOfFile, TextCompare: false) != 0)
                {
                    mControlFormBox.SelectedPatch(Strings.Split(text, ",")[1]);
                    continue;
                }
                break;
            }
        }

        private void Search2Customers(string svFind, bool UnSelected = true)
        {
            if (UnSelected)
            {
                mControlFormBox.UnSelectedAllClient();
                mControlFormBox.UnSelectedAllPatch();
            }
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = ListAccesClient.Items.GetEnumerator();
                while (enumerator.MoveNext())
                {
                    object objectValue = RuntimeHelpers.GetObjectValue(enumerator.Current);
                    if (File.ReadAllText(mPathAcces + AliasDic[Conversions.ToString(objectValue)] + mExtFileAcces, Encoding.Default).Contains(svFind))
                    {
                        ListSelectClient.Items.Add(RuntimeHelpers.GetObjectValue(objectValue));
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
            IEnumerator enumerator2 = default(IEnumerator);
            try
            {
                enumerator2 = ListSelectClient.Items.GetEnumerator();
                while (enumerator2.MoveNext())
                {
                    object objectValue2 = RuntimeHelpers.GetObjectValue(enumerator2.Current);
                    ListAccesClient.Items.Remove(RuntimeHelpers.GetObjectValue(objectValue2));
                }
            }
            finally
            {
                if (enumerator2 is IDisposable)
                {
                    (enumerator2 as IDisposable).Dispose();
                }
            }
            Label1.Text = "Customer List (" + Conversions.ToString(ListAccesClient.Items.Count) + "):";
            Label2.Text = "Selected customers (" + Conversions.ToString(ListSelectClient.Items.Count) + "):";
        }

        private void ConvBalise(string Balise)
        {
        }

        public string ArrayStringOf(ArrayList f, string findstring, int startindex = 0)
        {
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = f.GetEnumerator();
                while (enumerator.MoveNext())
                {
                    string text = Conversions.ToString(enumerator.Current);
                    int num = f.LastIndexOf(text);
                    if (text.Contains(findstring) && num > startindex)
                    {
                        return text.Replace(findstring, null);
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
            return null;
        }

        private bool CheckCRCForClient(string client)
        {
            string[] c = File.ReadAllLines(mPathAcces + AliasDic[client] + mExtFileAcces, Encoding.Default);
            ArrayList arrayList = new ArrayList();
            arrayList.Clear();
            arrayList.AddRange(c);
            string text = ArrayStringOf(arrayList, "%CRC%");
            if (Operators.CompareString(text, "", TextCompare: false) != 0)
            {
                int index = arrayList.IndexOf("%CRC%" + text);
                arrayList.RemoveAt(index);
                if (Operators.CompareString(mCheckSum.AppendString(ArrayListToString(arrayList)), text, TextCompare: false) == 0)
                {
                    return true;
                }
            }
            Interaction.MsgBox("Fichier \"" + client + "\" corrompu , veuillez contacter le support.", MsgBoxStyle.Critical, "Facility Management - " + MenuSoft + " - " + MenuPays);
            return false;
        }

        private string ArrayListToString(ArrayList list)
        {
            string text = null;
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = list.GetEnumerator();
                while (enumerator.MoveNext())
                {
                    string text2 = Conversions.ToString(enumerator.Current);
                    text = ((Operators.CompareString(text, null, TextCompare: false) != 0) ? (text + "\r\n" + text2) : text2);
                }
                return text;
            }
            finally
            {
                if (enumerator is IDisposable)
                {
                    (enumerator as IDisposable).Dispose();
                }
            }
        }

        private void AddAllCRC()
        {
        }

        private string AddCRC2String(string szString)
        {
            ArrayList arrayList = new ArrayList();
            arrayList.Clear();
            arrayList.AddRange(Strings.Split(szString, "\r\n"));
            string text = ArrayStringOf(arrayList, "%CRC%");
            if (Operators.CompareString(text, "", TextCompare: false) != 0)
            {
                int index = arrayList.IndexOf("%CRC%" + text);
                arrayList.RemoveAt(index);
            }
            string text2 = mCheckSum.AppendString(ArrayListToString(arrayList));
            if (Operators.CompareString(text2, text, TextCompare: false) != 0)
            {
                while (arrayList.LastIndexOf("") > 0)
                {
                    arrayList.Remove("");
                }
                arrayList.Add("%CRC%" + text2);
                szString = string.Join("\r\n", arrayList.ToArray(typeof(string)) as string[]);
            }
            return szString;
        }

        private void SearchPatch2Customers(ref string svPatch)
        {
        }

        private void ChargePatchFromFile()
        {
        }

        private void CMS_ListSelectClient_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            Disabled_FormBox();
            if (Operators.CompareString(mPathAcces, null, TextCompare: false) != 0)
            {
                string key = e.ClickedItem.Text;
                int num = cMSDic[key];
                string svClient = Conversions.ToString(ListSelectClient.SelectedItem);
                if (Operators.CompareString(svClient, null, TextCompare: false) != 0)
                {
                    svClient = AliasDic[svClient];
                }
                CMS_ListSelectClient.Hide();
                switch (num)
                {
                    case 1:
                        if (Operators.CompareString(svClient, null, TextCompare: false) != 0)
                        {
                            string svApplication = Environment.GetEnvironmentVariable("windir") + "\\notepad.exe";
                            string svCommandLine = mPathAcces + svClient + mExtFileAcces;
                            ProcessWindowStyle Style = ProcessWindowStyle.Normal;
                            byte WaitInfinity = 0;
                            mLaunchAppAndWait.LaunchAppAndWait(svApplication, svCommandLine,  Style,  WaitInfinity);
                        }
                        break;
                    case 5:
                        if (Operators.CompareString(svClient, null, TextCompare: false) != 0)
                        {
                            mControlFormBox.UnSelectedClient();
                        }
                        break;
                    case 6:
                        mControlFormBox.UnSelectedAllClient();
                        break;
                    case 9:
                        AddPatchToClient(svClient);
                        break;
                    case 10:
                        Send2FTP(svClient);
                        break;
                    case 14:
                        AddAllUpdate2Client(ref svClient);
                        break;
                    case 49:
                        SkipPatchToClient(svClient);
                        break;
                    case 50:
                        NotSkipPatchToClient();
                        break;
                    case 52:
                        AdmPatchToClient(svClient);
                        break;
                    case 53:
                        NotAdmPatchToClient();
                        break;
                }
            }
            Enabled_FormBox();
        }

        private void CMS_ListSelectPatch_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            Disabled_FormBox();
            string key = e.ClickedItem.Text;
            int num = cMSDic[key];
            string text = Conversions.ToString(ListSelectPatch.SelectedItem);
            CMS_ListSelectPatch.Hide();
            switch (num)
            {
                case 1:
                    if (Operators.CompareString(text, null, TextCompare: false) != 0)
                    {
                        string svApplication = Environment.GetEnvironmentVariable("windir") + "\\notepad.exe";
                        string svCommandLine = mPathExes + mFunction.ParsePath(text, 5) + mExtFileInfo;
                        ProcessWindowStyle Style = ProcessWindowStyle.Normal;
                        byte WaitInfinity = 0;
                        mLaunchAppAndWait.LaunchAppAndWait(svApplication, svCommandLine,  Style,  WaitInfinity);
                    }
                    break;
                case 5:
                    if (Operators.CompareString(text, null, TextCompare: false) != 0)
                    {
                        mControlFormBox.UnSelectedPatch();
                    }
                    break;
                case 6:
                    mControlFormBox.UnSelectedAllPatch();
                    break;
                case 9:
                    AddPatchToClient(null, text);
                    break;
                case 10:
                    Send2FTP(null, text);
                    break;
                case 49:
                    SkipPatchToClient(null, text);
                    break;
                case 50:
                    NotSkipPatchToClient(null, text);
                    break;
                case 52:
                    AdmPatchToClient(null, text);
                    break;
                case 53:
                    NotAdmPatchToClient(null, text);
                    break;
            }
            Enabled_FormBox();
        }

        private void AddPatchToClient(string svOnlySelectedClient = null, string svOnlySeclectedPatch = null, bool MsgFinishVisible = true, string PatchLessOne = null)
        {
            Disabled_FormBox();
            if ((ListSelectClient.Items.Count == 0) | (ListSelectPatch.Items.Count == 0))
            {
                if (MsgFinishVisible)
                {
                    Interaction.MsgBox("No selection !", MsgBoxStyle.Exclamation, "Facility Management - Add the path for customers");
                }
                Enabled_FormBox();
                return;
            }
            if (MsgFinishVisible && Interaction.MsgBox("Are you sure you want to add the patch ?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo | MsgBoxStyle.DefaultButton2) == MsgBoxResult.No)
            {
                Interaction.MsgBox("No selection !", MsgBoxStyle.Exclamation, "Facility Management - Add the patch for customers");
                Enabled_FormBox();
                return;
            }
            ListBox listBox = new ListBox();
            if (Operators.CompareString(svOnlySeclectedPatch, null, TextCompare: false) == 0)
            {
                listBox = ListSelectPatch;
            }
            else
            {
                listBox.Items.Add(svOnlySeclectedPatch);
            }
            checked
            {
                PATCH_INFO[] array = new PATCH_INFO[listBox.Items.Count - 1 + 1];
                int num = 0;
                IEnumerator enumerator = default(IEnumerator);
                try
                {
                    enumerator = listBox.Items.GetEnumerator();
                    while (enumerator.MoveNext())
                    {
                        object objectValue = RuntimeHelpers.GetObjectValue(enumerator.Current);
                        string path = mPathExes + mFunction.ParsePath(Conversions.ToString(objectValue), 5) + mExtFileInfo;
                        string text = mPathExes + Conversions.ToString(objectValue);
                        array[num].patch = Conversions.ToString(objectValue);
                        array[num].size = Conversions.ToString(FileSystem.FileLen(text));
                        array[num].crc = mCheckSum.Append(text);
                        array[num].info = Strings.Replace(Strings.Join(File.ReadAllLines(path, Encoding.Default), "\\r\\n"), ",", ";");
                        num++;
                    }
                }
                finally
                {
                    if (enumerator is IDisposable)
                    {
                        (enumerator as IDisposable).Dispose();
                    }
                }
                PATCH_INFO pATCH_INFO = default(PATCH_INFO);
                if (Operators.CompareString(PatchLessOne, null, TextCompare: false) != 0)
                {
                    string path2 = mPathExes + mFunction.ParsePath(PatchLessOne, 5) + mExtFileInfo;
                    string text2 = mPathExes + PatchLessOne;
                    if (File.Exists(text2))
                    {
                        pATCH_INFO.patch = PatchLessOne;
                        pATCH_INFO.size = Conversions.ToString(FileSystem.FileLen(text2));
                        pATCH_INFO.crc = mCheckSum.Append(text2);
                        pATCH_INFO.info = Strings.Replace(Strings.Join(File.ReadAllLines(path2, Encoding.Default), "\\r\\n"), ",", ";");
                    }
                }
                ListBox listBox2 = new ListBox();
                if (Operators.CompareString(svOnlySelectedClient, null, TextCompare: false) == 0)
                {
                    listBox2 = ListSelectClient;
                }
                else
                {
                    listBox2.Items.Add(svOnlySelectedClient);
                }
                ArrayList arrayList = new ArrayList();
                IEnumerator enumerator2 = default(IEnumerator);
                try
                {
                    enumerator2 = listBox2.Items.GetEnumerator();
                    while (enumerator2.MoveNext())
                    {
                        object objectValue2 = RuntimeHelpers.GetObjectValue(enumerator2.Current);
                        if (!CheckCRCForClient(objectValue2.ToString()))
                        {
                            continue;
                        }
                        bool flag = false;
                        string text3 = mPathAcces + AliasDic[Conversions.ToString(objectValue2)] + mExtFileAcces;
                        string text4 = mPathAcces + AliasDic[Conversions.ToString(objectValue2)] + mExtFileAcces + mExtFileTmp;
                        string[] array2 = File.ReadAllLines(text3, Encoding.Default);
                        int num2 = Array.FindIndex(array2, FindByDescription);
                        string text5 = null;
                        if (num2 + 1 < array2.Count() && Operators.CompareString(array2[num2 + 1].Trim(), null, TextCompare: false) != 0)
                        {
                            int num3 = num2 + 1;
                            int num4 = array2.Count() - 1;
                            for (int i = num3; i <= num4; i++)
                            {
                                text5 = text5 + "\r\n" + array2[i];
                            }
                            array2 = (string[])Utils.CopyArray(array2, new string[num2 + 1]);
                        }
                        int num5 = 0;
                        if (num2 > 0)
                        {
                            num5 = (int)Math.Round(Conversion.Val(Strings.Split(array2[num2 - 1], ",")[0]));
                        }
                        array2 = (string[])Utils.CopyArray(array2, new string[num2 + num + 1]);
                        PATCH_INFO[] array3 = array;
                        for (int j = 0; j < array3.Length; j++)
                        {
                            PATCH_INFO pATCH_INFO2 = array3[j];
                            if (Operators.CompareString(PatchLessOne, null, TextCompare: false) != 0 && Operators.CompareString(pATCH_INFO.FindString(array2), null, TextCompare: false) == 0 && Operators.CompareString(pATCH_INFO.patch, null, TextCompare: false) != 0 && Interaction.MsgBox("this patch \"" + pATCH_INFO.patch + "\" is not present for the client: " + Conversions.ToString(objectValue2) + "\r\n\r\nAre you sure you want to add the patch (\"" + pATCH_INFO2.patch + "\") ?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo, "Patch Pige") == MsgBoxResult.No)
                            {
                                arrayList.Add(Conversions.ToString(objectValue2));
                                flag = false;
                                break;
                            }
                            Application.DoEvents();
                            string text6 = pATCH_INFO2.FindString(array2);
                            if (Operators.CompareString(text6, null, TextCompare: false) != 0)
                            {
                                Interaction.MsgBox("this patch \"" + pATCH_INFO2.patch + "\" (Number: " + text6 + ") is already present for the client: " + Conversions.ToString(objectValue2), MsgBoxStyle.Exclamation);
                                arrayList.Add(Conversions.ToString(objectValue2));
                            }
                            else
                            {
                                pATCH_INFO2.count = Conversions.ToString(num5 + 1);
                                array2[num2] = pATCH_INFO2.ToString();
                                num2++;
                                num5++;
                                if (!flag)
                                {
                                    flag = true;
                                }
                            }
                        }
                        if (flag)
                        {
                            array2[num2] = BaliseAccesEndOfFile;
                            array2 = (string[])Utils.CopyArray(array2, new string[num2 + 1]);
                            if (File.Exists(text4))
                            {
                                File.Delete(text4);
                            }
                            File.Move(text3, text4);
                            File.WriteAllText(text3, AddCRC2String(Strings.Join(array2, "\r\n") + text5), Encoding.Default);
                            if (File.Exists(text3) & (FileSystem.FileLen(text3) != 0))
                            {
                                File.Delete(text4);
                            }
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
                if (arrayList.Count > 0)
                {
                    IEnumerator enumerator3 = default(IEnumerator);
                    try
                    {
                        enumerator3 = arrayList.GetEnumerator();
                        while (enumerator3.MoveNext())
                        {
                            string text7 = Conversions.ToString(enumerator3.Current);
                            ListAccesClient.Items.Add(text7);
                            ListSelectClient.Items.Remove(text7);
                        }
                    }
                    finally
                    {
                        if (enumerator3 is IDisposable)
                        {
                            (enumerator3 as IDisposable).Dispose();
                        }
                    }
                    if (ListSelectClient.Items.Count == 0)
                    {
                        mControlFormBox.UnSelectedAllPatch();
                    }
                }
                if (MsgFinishVisible)
                {
                    Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal);
                }
                ToolStripStatusLabelFM.Text = null;
                ToolStripProgressBar1.Value = 0;
                Enabled_FormBox();
            }
        }

        private void AddSkipToClient(string svOnlySelectedClient = null, string svOnlySeclectedPatch = null, bool MsgFinishVisible = true, string PatchLessOne = null)
        {
            Disabled_FormBox();
            if ((ListSelectClient.Items.Count == 0) | (ListSelectPatch.Items.Count == 0))
            {
                Interaction.MsgBox("No selection !", MsgBoxStyle.Exclamation, "Facility Management - Add the path for customers");
                Enabled_FormBox();
                return;
            }
            if (MsgFinishVisible && Interaction.MsgBox("Are you sure you want to add the patch ?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo | MsgBoxStyle.DefaultButton2) == MsgBoxResult.No)
            {
                Interaction.MsgBox("No selection !", MsgBoxStyle.Exclamation, "Facility Management - Add the patch for customers");
                Enabled_FormBox();
                return;
            }
            ListBox listBox = new ListBox();
            if (Operators.CompareString(svOnlySeclectedPatch, null, TextCompare: false) == 0)
            {
                listBox = ListSelectPatch;
            }
            else
            {
                listBox.Items.Add(svOnlySeclectedPatch);
            }
            checked
            {
                PATCH_INFO[] array = new PATCH_INFO[listBox.Items.Count - 1 + 1];
                int num = 0;
                IEnumerator enumerator = default(IEnumerator);
                try
                {
                    enumerator = listBox.Items.GetEnumerator();
                    while (enumerator.MoveNext())
                    {
                        object objectValue = RuntimeHelpers.GetObjectValue(enumerator.Current);
                        string path = mPathExes + mFunction.ParsePath(Conversions.ToString(objectValue), 5) + mExtFileInfo;
                        string text = mPathExes + Conversions.ToString(objectValue);
                        array[num].patch = Conversions.ToString(objectValue);
                        array[num].size = Conversions.ToString(FileSystem.FileLen(text));
                        array[num].crc = mCheckSum.Append(text);
                        array[num].info = Strings.Replace(Strings.Join(File.ReadAllLines(path, Encoding.Default), "\\r\\n"), ",", ";");
                        num++;
                    }
                }
                finally
                {
                    if (enumerator is IDisposable)
                    {
                        (enumerator as IDisposable).Dispose();
                    }
                }
                PATCH_INFO pATCH_INFO = default(PATCH_INFO);
                if (Operators.CompareString(PatchLessOne, null, TextCompare: false) != 0)
                {
                    string path2 = mPathExes + mFunction.ParsePath(PatchLessOne, 5) + mExtFileInfo;
                    string text2 = mPathExes + PatchLessOne;
                    if (File.Exists(text2))
                    {
                        pATCH_INFO.patch = PatchLessOne;
                        pATCH_INFO.size = Conversions.ToString(FileSystem.FileLen(text2));
                        pATCH_INFO.crc = mCheckSum.Append(text2);
                        pATCH_INFO.info = Strings.Replace(Strings.Join(File.ReadAllLines(path2, Encoding.Default), "\\r\\n"), ",", ";");
                    }
                }
                ListBox listBox2 = new ListBox();
                if (Operators.CompareString(svOnlySelectedClient, null, TextCompare: false) == 0)
                {
                    listBox2 = ListSelectClient;
                }
                else
                {
                    listBox2.Items.Add(svOnlySelectedClient);
                }
                ArrayList arrayList = new ArrayList();
                IEnumerator enumerator2 = default(IEnumerator);
                try
                {
                    enumerator2 = listBox2.Items.GetEnumerator();
                    while (enumerator2.MoveNext())
                    {
                        object objectValue2 = RuntimeHelpers.GetObjectValue(enumerator2.Current);
                        if (!CheckCRCForClient(objectValue2.ToString()))
                        {
                            continue;
                        }
                        bool flag = false;
                        string text3 = mPathAcces + AliasDic[Conversions.ToString(objectValue2)] + mExtFileAcces;
                        string text4 = mPathAcces + AliasDic[Conversions.ToString(objectValue2)] + mExtFileAcces + mExtFileTmp;
                        string[] array2 = File.ReadAllLines(text3, Encoding.Default);
                        int num2 = Array.FindIndex(array2, FindByDescription);
                        string text5 = null;
                        if (num2 + 1 < array2.Count() && Operators.CompareString(array2[num2 + 1].Trim(), null, TextCompare: false) != 0)
                        {
                            int num3 = num2 + 1;
                            int num4 = array2.Count() - 1;
                            for (int i = num3; i <= num4; i++)
                            {
                                text5 = text5 + "\r\n" + array2[i];
                            }
                            array2 = (string[])Utils.CopyArray(array2, new string[num2 + 1]);
                        }
                        int num5 = 0;
                        if (num2 > 0)
                        {
                            num5 = (int)Math.Round(Conversion.Val(Strings.Split(array2[num2 - 1], ",")[0]));
                        }
                        array2 = (string[])Utils.CopyArray(array2, new string[num2 + num + 1]);
                        PATCH_INFO[] array3 = array;
                        for (int j = 0; j < array3.Length; j++)
                        {
                            PATCH_INFO pATCH_INFO2 = array3[j];
                            if (Operators.CompareString(PatchLessOne, null, TextCompare: false) != 0 && Operators.CompareString(pATCH_INFO.FindString(array2), null, TextCompare: false) == 0 && Operators.CompareString(pATCH_INFO.patch, null, TextCompare: false) != 0 && Interaction.MsgBox("this patch \"" + pATCH_INFO.patch + "\" is not present for the client: " + Conversions.ToString(objectValue2) + "\r\n\r\nAre you sure you want to add the patch (\"" + pATCH_INFO2.patch + "\") ?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo, "Patch Pige") == MsgBoxResult.No)
                            {
                                arrayList.Add(Conversions.ToString(objectValue2));
                                flag = false;
                                break;
                            }
                            Application.DoEvents();
                            string text6 = pATCH_INFO2.FindString(array2);
                            if (Operators.CompareString(text6, null, TextCompare: false) != 0)
                            {
                                Interaction.MsgBox("this patch \"" + pATCH_INFO2.patch + "\" (Number: " + text6 + ") is already present for the client: " + Conversions.ToString(objectValue2), MsgBoxStyle.Exclamation);
                                arrayList.Add(Conversions.ToString(objectValue2));
                            }
                            else
                            {
                                pATCH_INFO2.count = Conversions.ToString(num5 + 1);
                                array2[num2] = pATCH_INFO2.ToString();
                                num2++;
                                num5++;
                                if (!flag)
                                {
                                    flag = true;
                                }
                            }
                        }
                        if (flag)
                        {
                            array2[num2] = BaliseAccesEndOfFile;
                            array2 = (string[])Utils.CopyArray(array2, new string[num2 + 1]);
                            if (File.Exists(text4))
                            {
                                File.Delete(text4);
                            }
                            File.Move(text3, text4);
                            File.WriteAllText(text3, AddCRC2String(Strings.Join(array2, "\r\n") + text5), Encoding.Default);
                            if (File.Exists(text3) & (FileSystem.FileLen(text3) != 0))
                            {
                                File.Delete(text4);
                            }
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
                if (arrayList.Count > 0)
                {
                    IEnumerator enumerator3 = default(IEnumerator);
                    try
                    {
                        enumerator3 = arrayList.GetEnumerator();
                        while (enumerator3.MoveNext())
                        {
                            string text7 = Conversions.ToString(enumerator3.Current);
                            ListAccesClient.Items.Add(text7);
                            ListSelectClient.Items.Remove(text7);
                        }
                    }
                    finally
                    {
                        if (enumerator3 is IDisposable)
                        {
                            (enumerator3 as IDisposable).Dispose();
                        }
                    }
                    if (ListSelectClient.Items.Count == 0)
                    {
                        mControlFormBox.UnSelectedAllPatch();
                    }
                }
                if (MsgFinishVisible)
                {
                    Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal);
                }
                ToolStripStatusLabelFM.Text = null;
                ToolStripProgressBar1.Value = 0;
                Enabled_FormBox();
            }
        }

        private void Add2Customer(string svAdd, string svOnlySelectedClient = null, bool MsgFinishVisible = true)
        {
            Disabled_FormBox();
            if (ListSelectClient.Items.Count == 0)
            {
                if (MsgFinishVisible)
                {
                    Interaction.MsgBox("No selection !", MsgBoxStyle.Exclamation, "Facility Management - Add " + svAdd);
                }
                Enabled_FormBox();
                return;
            }
            ListBox listBox = new ListBox();
            if (Operators.CompareString(svOnlySelectedClient, null, TextCompare: false) == 0)
            {
                listBox = ListSelectClient;
            }
            else
            {
                listBox.Items.Add(svOnlySelectedClient);
            }
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = listBox.Items.GetEnumerator();
                while (enumerator.MoveNext())
                {
                    object objectValue = RuntimeHelpers.GetObjectValue(enumerator.Current);
                    if (CheckCRCForClient(objectValue.ToString()))
                    {
                        string path = mPathAcces + AliasDic[Conversions.ToString(objectValue)] + mExtFileAcces;
                        ArrayList arrayList = new ArrayList();
                        string expression = File.ReadAllText(path, Encoding.Default);
                        arrayList.Clear();
                        arrayList.AddRange(Strings.Split(expression, "\r\n"));
                        int num = arrayList.IndexOf(BaliseAccesEndOfFile);
                        if (arrayList.IndexOf(svAdd) <= num)
                        {
                            arrayList.Add(svAdd);
                            expression = Strings.Join(arrayList.ToArray(), "\r\n");
                            File.WriteAllText(path, AddCRC2String(expression), Encoding.Default);
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
            if (MsgFinishVisible)
            {
                Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal);
            }
            Enabled_FormBox();
        }

        private void Del2Customer(string svDel, string svOnlySelectedClient = null, bool MsgFinishVisible = true)
        {
        }

        private void DelPatchToClient(string svOnlySelectedClient = null, string svOnlySeclectedPatch = null)
        {
        }

        private void SkipPatchToClient(string svOnlySelectedClient = null, string svOnlySeclectedPatch = null)
        {
            Disabled_FormBox();
            if ((ListSelectClient.Items.Count == 0) | (ListSelectPatch.Items.Count == 0))
            {
                Interaction.MsgBox("No selection !", MsgBoxStyle.Exclamation, "Facility Management - Skip these patch for customers");
                Enabled_FormBox();
                return;
            }
            if (Interaction.MsgBox("Are you sure you want to skip these patch ?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo | MsgBoxStyle.DefaultButton2) == MsgBoxResult.No)
            {
                Enabled_FormBox();
                return;
            }
            ListBox listBox = new ListBox();
            if (Operators.CompareString(svOnlySeclectedPatch, null, TextCompare: false) == 0)
            {
                listBox = ListSelectPatch;
            }
            else
            {
                listBox.Items.Add(svOnlySeclectedPatch);
            }
            ListBox listBox2 = new ListBox();
            if (Operators.CompareString(svOnlySelectedClient, null, TextCompare: false) == 0)
            {
                listBox2 = ListSelectClient;
            }
            else
            {
                listBox2.Items.Add(svOnlySelectedClient);
            }
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = listBox2.Items.GetEnumerator();
                IEnumerator enumerator2 = default(IEnumerator);
                while (enumerator.MoveNext())
                {
                    object objectValue = RuntimeHelpers.GetObjectValue(enumerator.Current);
                    if (!CheckCRCForClient(objectValue.ToString()))
                    {
                        continue;
                    }
                    bool flag = false;
                    string path = mPathAcces + AliasDic[Conversions.ToString(objectValue)] + mExtFileAcces;
                    string[] array = File.ReadAllLines(path, Encoding.Default);
                    try
                    {
                        enumerator2 = listBox.Items.GetEnumerator();
                        while (enumerator2.MoveNext())
                        {
                            object objectValue2 = RuntimeHelpers.GetObjectValue(enumerator2.Current);
                            string[] array2 = array;
                            foreach (string text in array2)
                            {
                                if (Operators.CompareString(text, BaliseAccesEndOfFile, TextCompare: false) == 0)
                                {
                                    break;
                                }
                                string[] array3 = Strings.Split(text, ",");
                                if ((Operators.CompareString(array3[1], objectValue2.ToString(), TextCompare: false) == 0) & !array3[4].Contains(Skip))
                                {
                                    if (!flag)
                                    {
                                        flag = true;
                                    }
                                    array3[4] += Skip;
                                    array[Array.IndexOf(array, text)] = Strings.Join(array3, ",");
                                }
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
                    if (flag)
                    {
                        File.WriteAllText(path, AddCRC2String(Strings.Join(array, "\r\n")), Encoding.Default);
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
            Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal);
            Enabled_FormBox();
        }

        private void NotSkipPatchToClient(string svOnlySelectedClient = null, string svOnlySeclectedPatch = null)
        {
            Disabled_FormBox();
            if ((ListSelectClient.Items.Count == 0) | (ListSelectPatch.Items.Count == 0))
            {
                Interaction.MsgBox("No selection !", MsgBoxStyle.Exclamation, "Facility Management - Skip these patch for customers");
                Enabled_FormBox();
                return;
            }
            if (Interaction.MsgBox("Are you sure you want to not skip these patch ?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo | MsgBoxStyle.DefaultButton2) == MsgBoxResult.No)
            {
                Enabled_FormBox();
                return;
            }
            ListBox listBox = new ListBox();
            if (Operators.CompareString(svOnlySeclectedPatch, null, TextCompare: false) == 0)
            {
                listBox = ListSelectPatch;
            }
            else
            {
                listBox.Items.Add(svOnlySeclectedPatch);
            }
            ListBox listBox2 = new ListBox();
            if (Operators.CompareString(svOnlySelectedClient, null, TextCompare: false) == 0)
            {
                listBox2 = ListSelectClient;
            }
            else
            {
                listBox2.Items.Add(svOnlySelectedClient);
            }
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = listBox2.Items.GetEnumerator();
                IEnumerator enumerator2 = default(IEnumerator);
                while (enumerator.MoveNext())
                {
                    object objectValue = RuntimeHelpers.GetObjectValue(enumerator.Current);
                    if (!CheckCRCForClient(objectValue.ToString()))
                    {
                        continue;
                    }
                    bool flag = false;
                    string path = mPathAcces + AliasDic[Conversions.ToString(objectValue)] + mExtFileAcces;
                    string[] array = File.ReadAllLines(path, Encoding.Default);
                    try
                    {
                        enumerator2 = listBox.Items.GetEnumerator();
                        while (enumerator2.MoveNext())
                        {
                            object objectValue2 = RuntimeHelpers.GetObjectValue(enumerator2.Current);
                            string[] array2 = array;
                            foreach (string text in array2)
                            {
                                if (Operators.CompareString(text, BaliseAccesEndOfFile, TextCompare: false) == 0)
                                {
                                    break;
                                }
                                string[] array3 = Strings.Split(text, ",");
                                if ((Operators.CompareString(array3[1], objectValue2.ToString(), TextCompare: false) == 0) & array3[4].Contains(Skip))
                                {
                                    if (!flag)
                                    {
                                        flag = true;
                                    }
                                    array3[4] = array3[4].Replace(Skip, "");
                                    array[Array.IndexOf(array, text)] = Strings.Join(array3, ",");
                                }
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
                    if (flag)
                    {
                        File.WriteAllText(path, AddCRC2String(Strings.Join(array, "\r\n")), Encoding.Default);
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
            Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal);
            Enabled_FormBox();
        }

        private void AdmPatchToClient(string svOnlySelectedClient = null, string svOnlySeclectedPatch = null)
        {
            Disabled_FormBox();
            if ((ListSelectClient.Items.Count == 0) | (ListSelectPatch.Items.Count == 0))
            {
                Interaction.MsgBox("No selection !", MsgBoxStyle.Exclamation, "Facility Management - Admin patch for customers");
                Enabled_FormBox();
                return;
            }
            if (Interaction.MsgBox("Are you sure you want to admin these patch ?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo | MsgBoxStyle.DefaultButton2) == MsgBoxResult.No)
            {
                Enabled_FormBox();
                return;
            }
            ListBox listBox = new ListBox();
            if (Operators.CompareString(svOnlySeclectedPatch, null, TextCompare: false) == 0)
            {
                listBox = ListSelectPatch;
            }
            else
            {
                listBox.Items.Add(svOnlySeclectedPatch);
            }
            ListBox listBox2 = new ListBox();
            if (Operators.CompareString(svOnlySelectedClient, null, TextCompare: false) == 0)
            {
                listBox2 = ListSelectClient;
            }
            else
            {
                listBox2.Items.Add(svOnlySelectedClient);
            }
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = listBox2.Items.GetEnumerator();
                IEnumerator enumerator2 = default(IEnumerator);
                while (enumerator.MoveNext())
                {
                    object objectValue = RuntimeHelpers.GetObjectValue(enumerator.Current);
                    if (!CheckCRCForClient(objectValue.ToString()))
                    {
                        continue;
                    }
                    bool flag = false;
                    string path = mPathAcces + AliasDic[Conversions.ToString(objectValue)] + mExtFileAcces;
                    string[] array = File.ReadAllLines(path, Encoding.Default);
                    try
                    {
                        enumerator2 = listBox.Items.GetEnumerator();
                        while (enumerator2.MoveNext())
                        {
                            object objectValue2 = RuntimeHelpers.GetObjectValue(enumerator2.Current);
                            string[] array2 = array;
                            foreach (string text in array2)
                            {
                                if (Operators.CompareString(text, BaliseAccesEndOfFile, TextCompare: false) == 0)
                                {
                                    break;
                                }
                                string[] array3 = Strings.Split(text, ",");
                                if ((Operators.CompareString(array3[1], objectValue2.ToString(), TextCompare: false) == 0) & !array3[4].Contains(Adm))
                                {
                                    if (!flag)
                                    {
                                        flag = true;
                                    }
                                    array3[4] += Adm;
                                    array[Array.IndexOf(array, text)] = Strings.Join(array3, ",");
                                }
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
                    if (flag)
                    {
                        File.WriteAllText(path, AddCRC2String(Strings.Join(array, "\r\n")), Encoding.Default);
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
            Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal);
            Enabled_FormBox();
        }

        private void NotAdmPatchToClient(string svOnlySelectedClient = null, string svOnlySeclectedPatch = null)
        {
            Disabled_FormBox();
            if ((ListSelectClient.Items.Count == 0) | (ListSelectPatch.Items.Count == 0))
            {
                Interaction.MsgBox("No selection !", MsgBoxStyle.Exclamation, "Facility Management - Admin patch for customers");
                Enabled_FormBox();
                return;
            }
            if (Interaction.MsgBox("Are you sure you want to not admin these patch ?", MsgBoxStyle.Exclamation | MsgBoxStyle.YesNo | MsgBoxStyle.DefaultButton2) == MsgBoxResult.No)
            {
                Enabled_FormBox();
                return;
            }
            ListBox listBox = new ListBox();
            if (Operators.CompareString(svOnlySeclectedPatch, null, TextCompare: false) == 0)
            {
                listBox = ListSelectPatch;
            }
            else
            {
                listBox.Items.Add(svOnlySeclectedPatch);
            }
            ListBox listBox2 = new ListBox();
            if (Operators.CompareString(svOnlySelectedClient, null, TextCompare: false) == 0)
            {
                listBox2 = ListSelectClient;
            }
            else
            {
                listBox2.Items.Add(svOnlySelectedClient);
            }
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = listBox2.Items.GetEnumerator();
                IEnumerator enumerator2 = default(IEnumerator);
                while (enumerator.MoveNext())
                {
                    object objectValue = RuntimeHelpers.GetObjectValue(enumerator.Current);
                    if (!CheckCRCForClient(objectValue.ToString()))
                    {
                        continue;
                    }
                    bool flag = false;
                    string path = mPathAcces + AliasDic[Conversions.ToString(objectValue)] + mExtFileAcces;
                    string[] array = File.ReadAllLines(path, Encoding.Default);
                    try
                    {
                        enumerator2 = listBox.Items.GetEnumerator();
                        while (enumerator2.MoveNext())
                        {
                            object objectValue2 = RuntimeHelpers.GetObjectValue(enumerator2.Current);
                            string[] array2 = array;
                            foreach (string text in array2)
                            {
                                if (Operators.CompareString(text, BaliseAccesEndOfFile, TextCompare: false) == 0)
                                {
                                    break;
                                }
                                string[] array3 = Strings.Split(text, ",");
                                if ((Operators.CompareString(array3[1], objectValue2.ToString(), TextCompare: false) == 0) & array3[4].Contains(Adm))
                                {
                                    if (!flag)
                                    {
                                        flag = true;
                                    }
                                    array3[4] = array3[4].Replace(Adm, "");
                                    array[Array.IndexOf(array, text)] = Strings.Join(array3, ",");
                                }
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
                    if (flag)
                    {
                        File.WriteAllText(path, AddCRC2String(Strings.Join(array, "\r\n")), Encoding.Default);
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
            Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal);
            Enabled_FormBox();
        }

        public bool FindByDescription(string item)
        {
            if (Operators.CompareString(item, BaliseAccesEndOfFile, TextCompare: false) == 0)
            {
                return true;
            }
            return false;
        }

        private void ToolTipInfo(ListBox list, MouseEventArgs e, string mPath, string mExt)
        {
            int num = list.IndexFromPoint(e.X, e.Y);
            if ((list.Items.Count > num) & (list.SelectedIndex != num))
            {
                list.SelectedIndex = num;
                string path = mPath + mFunction.ParsePath(Conversions.ToString(list.SelectedItem), 5) + mExt;
                if (File.Exists(path))
                {
                    string caption = File.ReadAllText(path, Encoding.Default);
                    ToolTip1.SetToolTip(list, caption);
                    Application.DoEvents();
                }
                else
                {
                    ToolTip1.SetToolTip(list, null);
                }
            }
            if (num == -1)
            {
                ToolTip1.SetToolTip(list, null);
            }
        }

        private void ToolTipInfoAlias(ListBox list, MouseEventArgs e)
        {
            int num = list.IndexFromPoint(e.X, e.Y);
            if (list.Items.Count > num)
            {
                list.SelectedIndex = num;
                if (Operators.CompareString(Conversions.ToString(list.SelectedItem), "", TextCompare: false) != 0)
                {
                    if (Operators.CompareString(AliasDic[Conversions.ToString(list.SelectedItem)], Conversions.ToString(list.SelectedItem), TextCompare: false) != 0)
                    {
                        ToolTip1.SetToolTip(list, AliasDic[Conversions.ToString(list.SelectedItem)]);
                        Application.DoEvents();
                    }
                    else
                    {
                        ToolTip1.SetToolTip(list, null);
                    }
                }
            }
            if (num == -1)
            {
                ToolTip1.SetToolTip(list, null);
            }
        }

        private void ListAccesClient_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                ListAccesClient_mDown = true;
            }
        }

        private void ListAccesClient_MouseLeave(object sender, EventArgs e)
        {
            if (!ListAccesClient_mDown)
            {
                ListAccesClient.SelectedIndex = -1;
            }
            else
            {
                ListAccesClient_mDown = false;
            }
        }

        private void ListAccesClient_MouseMove(object sender, MouseEventArgs e)
        {
            ToolTipInfoAlias(ListAccesClient, e);
        }

        private void ListSelectClient_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                ListSelectClient_mDown = true;
            }
        }

        private void ListSelectClient_MouseLeave(object sender, EventArgs e)
        {
            if (!ListSelectClient_mDown)
            {
                ListSelectClient.SelectedIndex = -1;
            }
            else
            {
                ListSelectClient_mDown = false;
            }
        }

        private void ListSelectClient_MouseMove(object sender, MouseEventArgs e)
        {
            ToolTipInfoAlias(ListSelectClient, e);
        }

        private void CMS_ListAccesClient_Opening(object sender, CancelEventArgs e)
        {
            CMS_ListAccesClient.Items.Clear();
            string text = Conversions.ToString(ListAccesClient.SelectedItem);
            if (Operators.CompareString(text, null, TextCompare: false) != 0)
            {
                text = AliasDic[text];
                if (Strings.StrComp(Conversions.ToString(ListAccesClient.SelectedItem), text, CompareMethod.Text) != 0)
                {
                    AliasCustomer = new ToolStripMenuItem
                    {
                        Text = cMSDic.Keys.ElementAtOrDefault(34),
                        Font = new Font("Tahoma", 16f, FontStyle.Regular, GraphicsUnit.Point, 0),
                        ImageScaling = ToolStripItemImageScaling.None,
                        AutoSize = false,
                        Size = new Size(160, 40),
                        Image = Resources.users_into
                    };
                    AliasCustomer.DropDownItems.Add(text.Replace("&", "&&"));
                    AliasCustomer.DropDownItems[0].ImageScaling = ToolStripItemImageScaling.None;
                    AliasCustomer.DropDownItems[0].Image = Resources.personne;
                    CMS_ListAccesClient.Items.AddRange(new ToolStripItem[1] { AliasCustomer });
                }
            }
            CMS_ListAccesClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(0), Resources.Create);
            checked
            {
                CMS_ListAccesClient.Items[CMS_ListAccesClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                string MotCle;
                if (Operators.CompareString(text, null, TextCompare: false) != 0)
                {
                    CMS_ListAccesClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(8), Resources.Dupliquer);
                    CMS_ListAccesClient.Items[CMS_ListAccesClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    CMS_ListAccesClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(3), Resources.Add);
                    CMS_ListAccesClient.Items[CMS_ListAccesClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    CMS_ListAccesClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(4), Resources.Ajouter);
                    CMS_ListAccesClient.Items[CMS_ListAccesClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    ref string menuSoft = ref MenuSoft;
                    MotCle = "Group1".Trim();
                    string text2 = mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref FacilityManagementIni).Trim();
                    if (Operators.CompareString(text2, null, TextCompare: false) != 0)
                    {
                        int num = 1;
                        Group = new ToolStripMenuItem
                        {
                            Text = cMSDic.Keys.ElementAtOrDefault(33),
                            Image = Resources.Add_all_icon
                        };
                        do
                        {
                            string[] array = Strings.Split(text2, ",");
                            Customer = new ToolStripMenuItem();
                            int num2 = array.Length - 1;
                            for (int i = 1; i <= num2; i++)
                            {
                                Customer.DropDownItems.Add(array[i].Replace("&", "&&"));
                                Customer.DropDownItems[i - 1].ImageScaling = ToolStripItemImageScaling.None;
                                Customer.DropDownItems[i - 1].Image = Resources.personne;
                            }
                            Group.DropDownItems.AddRange(new ToolStripItem[1] { Customer });
                            Group.DropDownItems[num - 1].Text = array[0];
                            Group.DropDownItems[num - 1].ImageScaling = ToolStripItemImageScaling.None;
                            Group.DropDownItems[num - 1].Image = Resources.utilisateurs;
                            num++;
                            ref string menuSoft2 = ref MenuSoft;
                            MotCle = "Group" + Conversions.ToString(num).Trim();
                            text2 = mFileIni.Select_GetIniString(ref menuSoft2, ref MotCle, ref FacilityManagementIni).Trim();
                        }
                        while (Operators.CompareString(text2, null, TextCompare: false) != 0);
                        CMS_ListAccesClient.Items.AddRange(new ToolStripItem[1] { Group });
                        CMS_ListAccesClient.Items[CMS_ListAccesClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    }
                    CMS_ListAccesClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(1), Resources.Edit);
                    CMS_ListAccesClient.Items[CMS_ListAccesClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    CMS_ListAccesClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(2), Resources.corbeille);
                    CMS_ListAccesClient.Items[CMS_ListAccesClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    CMS_ListAccesClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(14), Resources.Add_package);
                    CMS_ListAccesClient.Items[CMS_ListAccesClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                }
                CMS_ListAccesClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(40), Resources.Wizard_Wand);
                CMS_ListAccesClient.Items[CMS_ListAccesClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                ref string menuSoft3 = ref MenuSoft;
                MotCle = "ToolsMenuPige";
                if (Conversion.Val(mFileIni.Select_GetIniString(ref menuSoft3, ref MotCle, ref FacilityManagementIni).Trim()) == 1.0)
                {
                    CMS_ListAccesClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(20), Resources.Wizard_Wand);
                    CMS_ListAccesClient.Items[CMS_ListAccesClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                }
                Application.DoEvents();
            }
        }

        private void CMS_ListSelectClient_Opening(object sender, CancelEventArgs e)
        {
            CMS_ListSelectClient.Items.Clear();
            string text = Conversions.ToString(ListSelectClient.SelectedItem);
            if (Operators.CompareString(text, null, TextCompare: false) != 0)
            {
                text = AliasDic[text];
            }
            checked
            {
                if (Operators.CompareString(text, null, TextCompare: false) != 0)
                {
                    CMS_ListSelectClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(10), Resources.ftp);
                    CMS_ListSelectClient.Items[CMS_ListSelectClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    if (ListSelectPatch.Items.Count > 0)
                    {
                        CMS_ListSelectClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(9), Resources.telecharger);
                        CMS_ListSelectClient.Items[CMS_ListSelectClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    }
                    CMS_ListSelectClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(1), Resources.Edit);
                    CMS_ListSelectClient.Items[CMS_ListSelectClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    CMS_ListSelectClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(5), Resources.Supprimer);
                    CMS_ListSelectClient.Items[CMS_ListSelectClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                }
                CMS_ListSelectClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(41), Resources.import);
                CMS_ListSelectClient.Items[CMS_ListSelectClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                if (ListSelectClient.Items.Count > 0)
                {
                    CMS_ListSelectClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(36), Resources.document_a_exportation);
                    CMS_ListSelectClient.Items[CMS_ListSelectClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    CMS_ListSelectClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(42), Resources.couper_fichier);
                    CMS_ListSelectClient.Items[CMS_ListSelectClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    CMS_ListSelectClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(6), Resources.SupprimerAll);
                    CMS_ListSelectClient.Items[CMS_ListSelectClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                }
                if (Operators.CompareString(text, null, TextCompare: false) != 0)
                {
                    CMS_ListSelectClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(14), Resources.Add_package);
                    CMS_ListSelectClient.Items[CMS_ListSelectClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                }
                CMS_ListSelectClient.Items.Add(cMSDic.Keys.ElementAtOrDefault(21), Resources._Auto);
                CMS_ListSelectClient.Items[CMS_ListSelectClient.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                Application.DoEvents();
            }
        }

        private void CMS_ListSelectPatch_Opening(object sender, CancelEventArgs e)
        {
            CMS_ListSelectPatch.Items.Clear();
            checked
            {
                if (Operators.CompareString(Conversions.ToString(ListSelectPatch.SelectedItem), null, TextCompare: false) != 0)
                {
                    CMS_ListSelectPatch.Items.Add(cMSDic.Keys.ElementAtOrDefault(10), Resources.ftp);
                    CMS_ListSelectPatch.Items[CMS_ListSelectPatch.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    if (ListSelectClient.Items.Count > 0)
                    {
                        CMS_ListSelectPatch.Items.Add(cMSDic.Keys.ElementAtOrDefault(9), Resources.telecharger);
                        CMS_ListSelectPatch.Items[CMS_ListSelectPatch.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    }
                    CMS_ListSelectPatch.Items.Add(cMSDic.Keys.ElementAtOrDefault(1), Resources.Edit);
                    CMS_ListSelectPatch.Items[CMS_ListSelectPatch.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    CMS_ListSelectPatch.Items.Add(cMSDic.Keys.ElementAtOrDefault(5), Resources.Supprimer);
                    CMS_ListSelectPatch.Items[CMS_ListSelectPatch.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                }
                CMS_ListSelectPatch.Items.Add(cMSDic.Keys.ElementAtOrDefault(6), Resources.SupprimerAll);
                CMS_ListSelectPatch.Items[CMS_ListSelectPatch.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                Application.DoEvents();
            }
        }

        private void CMS_ListPatch_Opening(object sender, CancelEventArgs e)
        {
            CMS_ListPatch.Items.Clear();
            CMS_ListPatch.Items.Add(cMSDic.Keys.ElementAtOrDefault(0), Resources.Create);
            checked
            {
                CMS_ListPatch.Items[CMS_ListPatch.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                if (Operators.CompareString(Conversions.ToString(ListPatch.SelectedItem), null, TextCompare: false) != 0)
                {
                    CMS_ListPatch.Items.Add(cMSDic.Keys.ElementAtOrDefault(3), Resources.Add);
                    CMS_ListPatch.Items[CMS_ListPatch.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    CMS_ListPatch.Items.Add(cMSDic.Keys.ElementAtOrDefault(1), Resources.Edit);
                    CMS_ListPatch.Items[CMS_ListPatch.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    CMS_ListPatch.Items.Add(cMSDic.Keys.ElementAtOrDefault(7), Resources.Preview);
                    CMS_ListPatch.Items[CMS_ListPatch.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    CMS_ListPatch.Items.Add(cMSDic.Keys.ElementAtOrDefault(35), Resources.radar_wifi_icone);
                    CMS_ListPatch.Items[CMS_ListPatch.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                    CMS_ListPatch.Items.Add(cMSDic.Keys.ElementAtOrDefault(2), Resources.corbeille);
                    CMS_ListPatch.Items[CMS_ListPatch.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                }
                CMS_ListPatch.Items.Add(cMSDic.Keys.ElementAtOrDefault(19), Resources.reload);
                CMS_ListPatch.Items[CMS_ListPatch.Items.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                Application.DoEvents();
            }
        }

        private void Send2FTP(string svOnlySelectedClient = null, string svOnlySeclectedPatch = null, bool MsgFinishVisible = true)
        {
            checked
            {
                if (DateAndTime.DateDiff(DateInterval.Day, DIndex, DateTime.Now) > 0)
                {
                    ///WARNING
                    //_ = int.MaxValue * 2;
                }
                Disabled_FormBox();
                if ((ListSelectClient.Items.Count == 0) & (ListSelectPatch.Items.Count == 0))
                {
                    if (MsgFinishVisible)
                    {
                        Interaction.MsgBox("No selection !", MsgBoxStyle.Exclamation, "Facility Management - Transfer FTP Customers and patchs");
                    }
                    Enabled_FormBox();
                    return;
                }
                if (MsgFinishVisible && Interaction.MsgBox("Are you sure you want to patch the FTP server ?", MsgBoxStyle.YesNo | MsgBoxStyle.Information | MsgBoxStyle.DefaultButton2, "Facility Management - Transfer FTP Customers and patchs") == MsgBoxResult.No)
                {
                    Enabled_FormBox();
                    return;
                }
                ListBox listBox = new ListBox();
                if (Operators.CompareString(svOnlySeclectedPatch, null, TextCompare: false) == 0)
                {
                    listBox = ListSelectPatch;
                }
                else
                {
                    listBox.Items.Add(svOnlySeclectedPatch);
                }
                IEnumerator enumerator = default(IEnumerator);
                try
                {
                    enumerator = listBox.Items.GetEnumerator();
                    while (enumerator.MoveNext())
                    {
                        object objectValue = RuntimeHelpers.GetObjectValue(enumerator.Current);
                        string text = mPathExes + Conversions.ToString(objectValue);
                        string text2 = mCheckSum.Append(text);
                        string remoteFile = mFunction.ParsePath(Conversions.ToString(objectValue), 5) + "." + text2;
                        ref string menuSoft = ref MenuSoft;
                        string MotCle = "UpdateApps";
                        if (Conversion.Val(mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref FacilityManagementIni).Trim()) == 1.0)
                        {
                            string text3 = mFunction.ParsePath(Conversions.ToString(objectValue), 5) + ".Update";
                            string text4 = null;
                            if (Operators.CompareString(MenuSoft, "AutoUpdate", TextCompare: false) == 0)
                            {
                                text4 = ((Interaction.MsgBox("Voulez-vous afficher un message avant de faire la mise à jour pour tout le monde ? ", MsgBoxStyle.YesNo | MsgBoxStyle.Information | MsgBoxStyle.DefaultButton2, "Facility Management - Transfer FTP Customers and patchs") == MsgBoxResult.Yes) ? ";ALL" : ((Interaction.MsgBox("Voulez-vous afficher un message avant de faire la mise à jour pour les machines virtuelles ? ", MsgBoxStyle.YesNo | MsgBoxStyle.Information | MsgBoxStyle.DefaultButton2, "Facility Management - Transfer FTP Customers and patchs") != MsgBoxResult.Yes) ? null : ";VIRTUAL"));
                            }
                            ref string menuSoft2 = ref MenuSoft;
                            MotCle = "ActiveTesting";
                            if (Conversion.Val(mFileIni.Select_GetIniString(ref menuSoft2, ref MotCle, ref FacilityManagementIni).Trim()) == 1.0 && Interaction.MsgBox("Do you want to send for testing ? (" + Conversions.ToString(objectValue) + ")", MsgBoxStyle.YesNo | MsgBoxStyle.Information | MsgBoxStyle.DefaultButton2, "Facility Management - Transfer FTP Customers and patchs") == MsgBoxResult.Yes)
                            {
                                text3 += "Test";
                            }
                            string fileVersion = FileVersionInfo.GetVersionInfo(text).FileVersion;
                            double num = FileSystem.FileLen(text);
                            string contents = fileVersion + ";" + Conversions.ToString(num).Trim() + ";" + text2 + text4 + ";" + BaliseUpdateEndOfFile;
                            File.WriteAllText(mPathExes + text3, contents, Encoding.Default);
                            mConnection.MConnection_UploadFile(mPathExes + text3, text3);
                            File.Delete(mPathExes + text3);
                        }
                        ToolStripStatusLabelFM.Text = "Transfer FTP: " + Conversions.ToString(objectValue);
                        Application.DoEvents();
                        mConnection.MConnection_UploadFile(text, remoteFile);
                        Application.DoEvents();
                    }
                }
                finally
                {
                    if (enumerator is IDisposable)
                    {
                        (enumerator as IDisposable).Dispose();
                    }
                }
                ListBox listBox2 = new ListBox();
                if (Operators.CompareString(svOnlySelectedClient, null, TextCompare: false) == 0)
                {
                    listBox2 = ListSelectClient;
                }
                else
                {
                    listBox2.Items.Add(svOnlySelectedClient);
                }
                IEnumerator enumerator2 = default(IEnumerator);
                try
                {
                    enumerator2 = listBox2.Items.GetEnumerator();
                    while (enumerator2.MoveNext())
                    {
                        object objectValue2 = RuntimeHelpers.GetObjectValue(enumerator2.Current);
                        if (CheckCRCForClient(objectValue2.ToString()))
                        {
                            string fileName = mPathAcces + AliasDic[Conversions.ToString(objectValue2)] + mExtFileAcces;
                            string remoteFile2 = AliasDic[Conversions.ToString(objectValue2)] + "." + mCheckSum.AppendString(mEasyCrypt.Crypter(AliasDic[Conversions.ToString(objectValue2)], "sha1"));
                            ToolStripStatusLabelFM.Text = "Transfer FTP: " + Conversions.ToString(objectValue2) + mExtFileAcces;
                            Application.DoEvents();
                            mConnection.MConnection_UploadFile(fileName, remoteFile2);
                            Application.DoEvents();
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
                ToolStripStatusLabelFM.Text = "Transfer FTP: completed";
                Application.DoEvents();
                if (MsgFinishVisible)
                {
                    Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal);
                }
                ToolStripStatusLabelFM.Text = null;
                ToolStripProgressBar1.Value = 0;
                Application.DoEvents();
                Enabled_FormBox();
            }
        }

        private void CRC2File()
        {
            Disabled_FormBox();
            if ((ListSelectClient.Items.Count == 0) & (ListSelectPatch.Items.Count == 0))
            {
                Interaction.MsgBox("No selection !", MsgBoxStyle.Exclamation, "Facility Management - Creating CRC");
                Enabled_FormBox();
                return;
            }
            if (Interaction.MsgBox("Are you sure you want to create CRC file ?", MsgBoxStyle.YesNo | MsgBoxStyle.Information | MsgBoxStyle.DefaultButton2, "Facility Management - Creating CRC") == MsgBoxResult.No)
            {
                Enabled_FormBox();
                return;
            }
            ArrayList arrayList = new ArrayList();
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = ListSelectPatch.Items.GetEnumerator();
                while (enumerator.MoveNext())
                {
                    object objectValue = RuntimeHelpers.GetObjectValue(enumerator.Current);
                    arrayList.Add(RuntimeHelpers.GetObjectValue(objectValue));
                }
            }
            finally
            {
                if (enumerator is IDisposable)
                {
                    (enumerator as IDisposable).Dispose();
                }
            }
            IEnumerator enumerator2 = default(IEnumerator);
            try
            {
                enumerator2 = arrayList.GetEnumerator();
                while (enumerator2.MoveNext())
                {
                    object objectValue2 = RuntimeHelpers.GetObjectValue(enumerator2.Current);
                    mCheckSum.Append(mPathExes + Conversions.ToString(objectValue2));
                    ListSelectPatch.SelectedItem = RuntimeHelpers.GetObjectValue(objectValue2);
                    Application.DoEvents();
                }
            }
            finally
            {
                if (enumerator2 is IDisposable)
                {
                    (enumerator2 as IDisposable).Dispose();
                }
            }
            ToolStripStatusLabelFM.Text = "Creating CRC: completed";
            Application.DoEvents();
            Interaction.MsgBox("Completed.", MsgBoxStyle.Information | MsgBoxStyle.SystemModal);
            ToolStripStatusLabelFM.Text = null;
            ToolStripProgressBar1.Value = 0;
            Application.DoEvents();
            Enabled_FormBox();
        }

        public string[] GetFiles(string SourceFolder, string Filter, SearchOption searchOption)
        {
            ArrayList arrayList = new ArrayList();
            string[] array = Filter.Split('|');
            foreach (string searchPattern in array)
            {
                arrayList.AddRange(Directory.GetFiles(SourceFolder, searchPattern, searchOption));
            }
            return (string[])arrayList.ToArray(Type.GetType("System.String"));
        }

        public void CreateListExes()
        {
            ListPatch.Items.Clear();
            ListSelectPatch.Items.Clear();
            DicExes.Clear();
            string[] array = (from w in (from x in new DirectoryInfo(mPathExes).GetFiles("*.exe", SearchOption.TopDirectoryOnly)
                                         orderby x.LastWriteTime descending
                                         select x.FullName).ToArray()
                              select mFunction.ParsePath(w, 4)).ToArray();
            foreach (string text in array)
            {
                ListPatch.Items.Add(text);
                DicExes.Add(text, value: true);
            }
            Label3.Text = "Selected Updates (" + Conversions.ToString(ListSelectPatch.Items.Count) + ") :  ";
            Label4.Text = "Updates List (" + Conversions.ToString(ListPatch.Items.Count) + "):";
        }

        private void InvertClient()
        {
            foreach (KeyValuePair<string, string> item in AliasDic)
            {
                string key = item.Key;
                if (ListSelectClient.Items.Contains(key))
                {
                    ListAccesClient.Items.Add(key);
                    ListSelectClient.Items.Remove(key);
                }
                else
                {
                    ListSelectClient.Items.Add(key);
                    ListAccesClient.Items.Remove(key);
                }
            }
        }

        private void ListPatch_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                ListPatch_mDown = true;
            }
        }

        private void ListPatch_MouseLeave(object sender, EventArgs e)
        {
            if (!ListPatch_mDown)
            {
                ListPatch.SelectedIndex = -1;
            }
            else
            {
                ListPatch_mDown = false;
            }
        }

        private void ListPatch_MouseMove(object sender, MouseEventArgs e)
        {
            ToolTipInfo(ListPatch, e, mPathExes, mExtFileInfo);
        }

        private void ListSelectPatch_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                ListSelectPatch_mDown = true;
            }
        }

        private void ListSelectPatch_MouseLeave(object sender, EventArgs e)
        {
            if (!ListSelectPatch_mDown)
            {
                ListSelectPatch.SelectedIndex = -1;
            }
            else
            {
                ListSelectPatch_mDown = false;
            }
        }

        private void ListSelectPatch_MouseMove(object sender, MouseEventArgs e)
        {
            ToolTipInfo(ListSelectPatch, e, mPathExes, mExtFileInfo);
        }

        private void SoftwareMenu_DropDownOpening(object sender, EventArgs e)
        {
            SoftwareMenu.DropDownItems.Clear();
            checked
            {
                foreach (string key in SoftwareDic.Keys)
                {
                    foreach (string value in CountryDic.Values)
                    {
                        if (Directory.Exists(SoftwareDic[key] + value))
                        {
                            Image image;
                            try
                            {
                                image = DicSoftImage[key];
                            }
                            catch (Exception ex)
                            {
                                ProjectData.SetProjectError(ex);
                                Exception ex2 = ex;
                                image = Resources.avertissement;
                                ProjectData.ClearProjectError();
                            }
                            SoftwareMenu.DropDownItems.Add(key, image);
                            SoftwareMenu.DropDownItems[SoftwareMenu.DropDownItems.Count - 1].ImageScaling = ToolStripItemImageScaling.None;
                            SoftwareMenu.DropDownItems[SoftwareMenu.DropDownItems.Count - 1].BackColor = backcolor2;
                            break;
                        }
                    }
                }
                if (SoftwareMenu.DropDownItems.Count == 1)
                {
                    MenuSoft = SoftwareMenu.DropDownItems[0].Text;
                    SoftwareMenu.DropDownItems.Clear();
                }
            }
        }

        private void FiltrePatch_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Escape)
            {
                FiltrePatch.Text = "";
            }
        }

        private void FiltrePatch_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (FilterSupp)
            {
                FiltrePatch.BackColor = Color.FromKnownColor(KnownColor.Window);
                FiltrePatch.ForeColor = Color.FromKnownColor(KnownColor.WindowText);
                FilterSupp = false;
            }
            else
            {
                FiltrePatch.BackColor = Color.FromKnownColor(KnownColor.Red);
                FiltrePatch.ForeColor = Color.FromKnownColor(KnownColor.White);
                FilterSupp = true;
            }
            if (Operators.CompareString(FiltrePatch.Text, null, TextCompare: false) != 0)
            {
                PatchFilter();
            }
        }

        private void FiltrePatch_TextChanged(object sender, EventArgs e)
        {
            PatchFilter();
        }

        private void PatchFilter()
        {
            string text = FiltrePatch.Text;
            ListPatch.Items.Clear();
            string[] array = new string[checked(DicExes.Keys.Count - 1 + 1)];
            DicExes.Keys.CopyTo(array, 0);
            string[] array2 = array;
            foreach (string text2 in array2)
            {
                if (Operators.CompareString(text, null, TextCompare: false) != 0)
                {
                    if (Conversions.ToBoolean(Interaction.IIf(FilterSupp, Strings.InStr(text2, text, CompareMethod.Text) == 0, Strings.InStr(text2, text, CompareMethod.Text) > 0)) & !ListSelectPatch.Items.Contains(text2))
                    {
                        ListPatch.Items.Add(text2);
                        DicExes[text2] = true;
                    }
                    else
                    {
                        DicExes[text2] = false;
                    }
                }
                else if (!ListSelectPatch.Items.Contains(text2))
                {
                    ListPatch.Items.Add(text2);
                    DicExes[text2] = true;
                }
                else
                {
                    DicExes[text2] = false;
                }
            }
            Label3.Text = "Selected Updates (" + Conversions.ToString(ListSelectPatch.Items.Count) + "):";
            Label4.Text = "Updates List (" + Conversions.ToString(ListPatch.Items.Count) + "):";
        }

        private void ButtonFiltreUpdates_Click(object sender, EventArgs e)
        {
            FiltrePatch.BackColor = Color.FromKnownColor(KnownColor.Window);
            FiltrePatch.ForeColor = Color.FromKnownColor(KnownColor.WindowText);
            FilterSupp = false;
            FiltrePatch.Text = null;
        }

        private Dictionary<string, string> ReadSql()
        {
            Dictionary<string, string> dictionary = new Dictionary<string, string>();
            if (Operators.CompareString(SQL_Serveur, "", TextCompare: false) == 0)
            {
                return dictionary;
            }
            ref string menuSoft = ref MenuSoft;
            string MotCle = "database";
            string text = mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref FacilityManagementIni);
            if (Operators.CompareString(text, "", TextCompare: false) != 0)
            {
                try
                {
                    string connectionString = "database=" + text + ";server=" + SQL_Serveur + ";User ID=" + SQL_Login + ";pwd=" + SQL_Password;
                    SqlConnection sqlConnection = new SqlConnection(connectionString);
                    sqlConnection.Open();
                    SqlDataReader sqlDataReader = new SqlCommand("SELECT Societe.Societe_Nom, Pays.Pays_Name from Poste inner join Societe ON Poste.Poste_Societe = Societe.Societe_Id INNER JOIN Pays ON Societe.Societe_Pays = Pays.Pays_Id WHERE Poste.Poste_HardwareCode <> '0' and Poste.Poste_Supp = 0 ORDER BY Societe.Societe_Nom ASC", sqlConnection).ExecuteReader();
                    object[] array = new object[2];
                    while (sqlDataReader.Read())
                    {
                        sqlDataReader.GetValues(array);
                        string text2 = Conversions.ToString(array[0]);
                        if (!dictionary.ContainsKey(text2) & !text2.Contains("JFC Test"))
                        {
                            dictionary.Add(text2, Conversions.ToString(array[1]));
                        }
                    }
                    sqlDataReader.Close();
                    sqlConnection.Close();
                }
                catch (Exception ex)
                {
                    ProjectData.SetProjectError(ex);
                    Exception ex2 = ex;
                    if (!SQL_NoErrorMessage)
                    {
                        Interaction.MsgBox(ex2.Message, MsgBoxStyle.Critical, "Facility Management - " + MenuSoft + " - " + MenuPays);
                    }
                    ProjectData.ClearProjectError();
                }
            }
            return dictionary;
        }

        private Dictionary<string, string> ReadSqlCreate()
        {
            Dictionary<string, string> dictionary = new Dictionary<string, string>();
            if (Operators.CompareString(SQL_Serveur, "", TextCompare: false) == 0)
            {
                return dictionary;
            }
            ref string menuSoft = ref MenuSoft;
            string MotCle = "database";
            string text = mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref FacilityManagementIni);
            if (Operators.CompareString(text, "", TextCompare: false) != 0)
            {
                try
                {
                    string connectionString = "database=" + text + ";server=" + SQL_Serveur + ";User ID=" + SQL_Login + ";pwd=" + SQL_Password;
                    SqlConnection sqlConnection = new SqlConnection(connectionString);
                    sqlConnection.Open();
                    SqlDataReader sqlDataReader = new SqlCommand("SELECT Societe.Societe_Nom from Societe INNER JOIN Pays ON Societe.Societe_Pays = Pays.Pays_Id where Pays.Pays_Name like '" + MenuPays + "' ORDER BY Societe.Societe_Nom ASC", sqlConnection).ExecuteReader();
                    object[] array = new object[1];
                    while (sqlDataReader.Read())
                    {
                        sqlDataReader.GetValues(array);
                        string text2 = Conversions.ToString(array[0]);
                        if (!dictionary.ContainsKey(text2) & !text2.Contains("JFC Test"))
                        {
                            dictionary.Add(text2, MenuPays);
                        }
                    }
                    sqlDataReader.Close();
                    sqlConnection.Close();
                }
                catch (Exception ex)
                {
                    ProjectData.SetProjectError(ex);
                    Exception ex2 = ex;
                    if (!SQL_NoErrorMessage)
                    {
                        Interaction.MsgBox(ex2.Message, MsgBoxStyle.Critical, "Facility Management - " + MenuSoft + " - " + MenuPays);
                    }
                    ProjectData.ClearProjectError();
                }
            }
            return dictionary;
        }

        private void Enabled_FormBox()
        {
            MenuStripFM.Enabled = true;
            ListAccesClient.Enabled = true;
            ListSelectPatch.Enabled = true;
            ListSelectClient.Enabled = true;
            ListPatch.Enabled = true;
            FiltrePatch.Enabled = true;
            Application.DoEvents();
        }

        private void Disabled_FormBox()
        {
            UpdateMenu.HideDropDown();
            ToolsMenu.HideDropDown();
            SoftwareMenu.HideDropDown();
            CountryMenu.HideDropDown();
            CustomerMenu.HideDropDown();
            ExitMenu.HideDropDown();
            MenuStripFM.Enabled = false;
            ListAccesClient.Enabled = false;
            ListSelectPatch.Enabled = false;
            ListSelectClient.Enabled = false;
            ListPatch.Enabled = false;
            FiltrePatch.Enabled = false;
            Application.DoEvents();
        }

        private void DelAllPatchBeforeToClient()
        {
        }

        private void Reindexing()
        {
        }

        private void Group_DropDownItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            CMS_ListAccesClient.Hide();
            Disabled_FormBox();
            string sGroup = e.ClickedItem.Text;
            AddGroup(sGroup);
            Enabled_FormBox();
        }

        private void AddGroup(string sGroup = null)
        {
            int num = 1;
            checked
            {
                while (true)
                {
                    ref string menuSoft = ref MenuSoft;
                    string MotCle = "Group" + Conversions.ToString(num).Trim();
                    string text = mFileIni.Select_GetIniString(ref menuSoft, ref MotCle, ref FacilityManagementIni).Trim();
                    if (Operators.CompareString(text, null, TextCompare: false) == 0)
                    {
                        break;
                    }
                    string[] array = Strings.Split(text, ",");
                    if ((Operators.CompareString(array[0], sGroup, TextCompare: false) == 0) | (Operators.CompareString(sGroup, null, TextCompare: false) == 0))
                    {
                        int num2 = array.Length - 1;
                        for (int i = 1; i <= num2; i++)
                        {
                            mControlFormBox.SelectedClient(array[i].Trim());
                        }
                        if (Operators.CompareString(array[0], sGroup, TextCompare: false) == 0)
                        {
                            break;
                        }
                    }
                    num++;
                }
            }
        }

        public void HttpSendMessage()
        {
        }

        private bool ReadSqlExt(string svClient, string svPatch, string Etat = "1")
        {
            if ((Operators.CompareString(SQL_SiteInternet_Serveur, "", TextCompare: false) == 0) | !bAdvancedMenu)
            {
                return false;
            }
            try
            {
                svClient = AliasDic[svClient];
                svClient = Strings.Replace(svClient, "é", "?");
                svClient = Strings.Replace(svClient, "è", "?");
                string connectionString = "database=siteinternet;server=" + SQL_SiteInternet_Serveur + ";User ID=" + SQL_SiteInternet_Login + ";pwd=" + SQL_SiteInternet_Password;
                SqlConnection sqlConnection = new SqlConnection(connectionString);
                sqlConnection.Open();
                SqlDataReader sqlDataReader = new SqlCommand("Select DISTINCT maj from Log where  login = '" + svClient + "' and  maj like '%" + svPatch + ".exe' and logiciel = '" + MenuSoft + "' and etat = '" + Etat + "'", sqlConnection).ExecuteReader();
                bool hasRows = sqlDataReader.HasRows;
                sqlDataReader.Close();
                sqlConnection.Close();
                return hasRows;
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                bool hasRows = false;
                if (!SQL_NoErrorMessage)
                {
                    Interaction.MsgBox(ex2.Message, MsgBoxStyle.Critical, "Facility Management - " + MenuSoft + " - " + MenuPays);
                }
                ProjectData.ClearProjectError();
                return hasRows;
            }
        }

        private void SearchPatch2DataBase(ref string svPatch)
        {
        }

        private void TaskApps(string sztaskApplication = "", bool Kill = false)
        {
        }

        private void FormBoxResize()
        {
            //Discarded unreachable code: IL_012a, IL_0160, IL_0162, IL_0172, IL_0194
            int num = default(int);
            int num4 = default(int);
            try
            {
                ProjectData.ClearProjectError();
                num = 1;
                int num2 = 2;
                checked
                {
                    int num3 = base.Size.Width - 16;
                    num2 = 3;
                    CustomerMenu.Width = (int)Conversion.Int((double)num3 * 0.18);
                    num2 = 4;
                    UpdateMenu.Width = (int)Conversion.Int((double)num3 * 0.13);
                    num2 = 5;
                    ToolsMenu.Width = (int)Conversion.Int((double)num3 * 0.13);
                    num2 = 6;
                    CountryMenu.Width = (int)Conversion.Int((double)num3 * 0.16);
                    num2 = 7;
                    ExitMenu.Width = (int)Conversion.Int((double)num3 * 0.13);
                    num2 = 8;
                    SoftwareMenu.Width = num3 - CustomerMenu.Width - UpdateMenu.Width - ToolsMenu.Width - CountryMenu.Width - ExitMenu.Width;
                    num2 = 9;
                    ToolStripProgressBar1.Size = new Size((int)Conversion.Int((double)num3 * 0.2), 16);
                }
            }
            catch (Exception obj) when (obj is Exception && num != 0 && num4 == 0)
            {
                ProjectData.SetProjectError((Exception)obj);
                /*Error near IL_0192: Could not find block for branch target IL_0162*/
                ;
            }
            if (num4 != 0)
            {
                ProjectData.ClearProjectError();
            }
        }

        private void FormBox_Resize(object sender, EventArgs e)
        {
            if (base.Visible == base.Enabled)
            {
                FormBoxResize();
            }
        }

        private void F10_Click(object sender, EventArgs e)
        {
            string svPatch = Conversions.ToString(ListPatch.SelectedItem);
            if (Operators.CompareString(svPatch, null, TextCompare: false) != 0)
            {
                SearchPatch2DataBase(ref svPatch);
            }
        }

        private void F9_Click(object sender, EventArgs e)
        {
            string svPatch = Conversions.ToString(ListPatch.SelectedItem);
            if (Operators.CompareString(svPatch, null, TextCompare: false) != 0)
            {
                SearchPatch2Customers(ref svPatch);
            }
        }

        private void F6_Click(object sender, EventArgs e)
        {
            AddPatchToClient(null, null, MsgFinishVisible: false);
            Send2FTP(null, null, MsgFinishVisible: false);
        }

        private void F5_Click(object sender, EventArgs e)
        {
            Init_Software();
            GetCustomerOfCountry(F5: true);
        }

        private void F4_Click(object sender, EventArgs e)
        {
            string text = Conversions.ToString(ListAccesClient.SelectedItem);
            if (Operators.CompareString(text, null, TextCompare: false) != 0)
            {
                text = AliasDic[text];
            }
            else
            {
                text = Conversions.ToString(ListSelectClient.SelectedItem);
                if (Operators.CompareString(text, null, TextCompare: false) != 0)
                {
                    text = AliasDic[text];
                }
            }
            if (Operators.CompareString(text, null, TextCompare: false) != 0)
            {
                string svApplication = Environment.GetEnvironmentVariable("windir") + "\\notepad.exe";
                string svCommandLine = mPathAcces + text + mExtFileAcces;
                ProcessWindowStyle Style = ProcessWindowStyle.Normal;
                byte WaitInfinity = 0;
                mLaunchAppAndWait.LaunchAppAndWait(svApplication, svCommandLine, Style, WaitInfinity);
            }
        }

        private void F3_Click(object sender, EventArgs e)
        {
            checked
            {
                if (DateAndTime.DateDiff(DateInterval.Day, DIndex, DateTime.Now) > 0)
                {
                    //_ = int.MaxValue * 2;
                    throw new Exception("Problem code translator");
                }
                MyProject.Forms.CreateUpdate.ShowDialog();
            }
        }

        private void F2_Click(object sender, EventArgs e)
        {
            string text = Interaction.InputBox("New Client:").Trim();
            if (Operators.CompareString(text, null, TextCompare: false) == 0)
            {
                return;
            }
            text.Trim();
            AliasCompany(text);
            foreach (KeyValuePair<string, string> item in CountryDic)
            {
                if (File.Exists(mPathExes + item.Value + text + mExtFileAcces))
                {
                    Interaction.MsgBox("The client '" + text + "' already exists in'" + item.Key + "'.", MsgBoxStyle.Exclamation, "Facility Management - New Client");
                    Enabled_FormBox();
                    return;
                }
            }
            string path = mPathAcces + text + mExtFileAcces;
            if (File.Exists(path))
            {
                File.Delete(path);
            }
            File.WriteAllText(path, AddCRC2String(BaliseAccesEndOfFile), Encoding.Default);
            ListAccesClient.Items.Add(text);
        }

        private void SplitContainer3_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            SplitContainer3.SplitterDistance = checked((int)Math.Round((double)SplitContainer3.Size.Width / 2.0));
        }

        private void SplitContainer2_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            SplitContainer2.SplitterDistance = checked((int)Math.Round((double)SplitContainer2.Size.Width / 2.0));
        }

        private void SplitContainer4_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            SplitContainer4.SplitterDistance = checked((int)Math.Round((double)SplitContainer4.Size.Width / 2.0));
        }

        //private void AddCRC2Files()
        //{
        //    _ = bAdvancedMenu;
        //}

        //private void AddEXE2Files()
        //{
        //}
    }
}
