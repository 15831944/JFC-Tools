using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security.Principal;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ReStartAffinageAuto
{
    public partial class Form1 : Form
    {
        // Fields
        private string TargetConso;
        private string TargetAffinageAuto;
        private string szFileForceReStart;
        private string szUserFile;
        private string szBaseConso;
        private const short MAX_LEN = 640;

        // Methods
        public Form1()
        {
            base.Load += new EventHandler(this.Form1_Load);
            this.TargetConso = @"\\Parappp01112\ARTRADIO\CONSOLIDATION\";
            this.TargetAffinageAuto = @"\\Parappp01112\ARTRADIO\AFFINAGEAUTO\";
            this.szFileForceReStart = this.TargetAffinageAuto + "ConsoAffinageAuto.rst";
            this.szUserFile = this.TargetConso + "user.txt";
            this.szBaseConso = this.TargetConso + "KeyConso.db";
            this.InitializeComponent();
        }

        private string Conversion(long TempsTotal)
        {
            string str;
            if (TempsTotal > 0x1_5180L)
            {
                str = "00:00:00";
            }
            else
            {
                int expression = (int)Convert.ToInt32((double)(((double)TempsTotal) / 0xe10));
                int num2 = (int)Convert.ToInt32((double)(((double)(TempsTotal - (0xe10 * expression))) / 60.0));
                int num3 = (((int)TempsTotal) - (0xe10 * expression)) - (60 * num2);
                string[] textArray1 = new string[] { expression.ToString("##00"), ":", num2.ToString("##00"), ":", num3.ToString("##00") };
                str = string.Concat(textArray1);
            }
            return str;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.refresh_status();
        }

        [DllImport("kernel32", EntryPoint = "GetPrivateProfileStringA", CharSet = CharSet.Ansi, SetLastError = true, ExactSpelling = true)]
        private static extern short GetPrivateProfileString([MarshalAs(UnmanagedType.VBByRefStr)] ref string NomModule, [MarshalAs(UnmanagedType.VBByRefStr)] ref string MotCle, [MarshalAs(UnmanagedType.VBByRefStr)] ref string StringParDefaut, [MarshalAs(UnmanagedType.VBByRefStr)] ref string ReturnedString, short Longueur, [MarshalAs(UnmanagedType.VBByRefStr)] ref string NomFichierIni);


        [MethodImpl(MethodImplOptions.NoOptimization | MethodImplOptions.NoInlining)]
        private void Quitter_Click(object sender, EventArgs e)
        {
            base.Close();
            //ProjectData.EndApp();
        }

        private void Rafraichir_Click(object sender, EventArgs e)
        {
            this.refresh_status();
        }

        private void refresh_status()
        {
            base.Show();
            this.Start.Enabled = false;
            this.Rafraichir.Enabled = false;
            if (File.Exists(this.szFileForceReStart))
            {
                this.LabelValueStatus.Text = "En attente de traitement...";
                Application.DoEvents();
                while (true)
                {
                    if (!File.Exists(this.szFileForceReStart))
                    {
                        while (!File.Exists(this.szUserFile))
                        {
                            Application.DoEvents();
                            Thread.Sleep(500);
                        }
                        break;
                    }
                    Application.DoEvents();
                    Thread.Sleep(500);
                }
            }
            if (File.Exists(this.szUserFile))
            {
                this.LabelValueUser.Text = File.ReadAllLines(this.szUserFile, Encoding.UTF8).First<string>();
                this.LabelValueDate.Text = File.GetLastWriteTime(this.szUserFile).ToString();
                this.LabelValueStatus.Text = "En cours de traitement...";
                while (true)
                {
                    if (!File.Exists(this.szUserFile))
                    {
                        this.Rafraichir.Text = "Rafraichir";
                        break;
                    }
                    try
                    {
                        DateTime lastWriteTime = File.GetLastWriteTime(this.szUserFile);
                        //string strB = this.Conversion(DateAndTime.DateDiff(DateInterval.Second, lastWriteTime, DateTime.Now, FirstDayOfWeek.Sunday, FirstWeekOfYear.Jan1));
                        string strB = (lastWriteTime - DateTime.Now).TotalSeconds.ToString();
                        if (this.Rafraichir.Text != strB)
                        {
                            this.Rafraichir.Text = strB;
                        }
                    }
                    catch (Exception exception1)
                    {
                        Exception ex = exception1;
                        //ProjectData.SetProjectError(ex);
                        Exception exception = ex;
                        //ProjectData.ClearProjectError();
                    }
                    Application.DoEvents();
                    Thread.Sleep(100);
                }
            }
            this.LabelValueUser.Text = "Aucun";
            this.LabelValueDate.Text = File.GetLastWriteTime(this.szBaseConso).ToString();
            this.LabelValueStatus.Text = "traitement termin\x00e9.";
            this.Start.Enabled = true;
            this.Rafraichir.Enabled = true;
        }

        public string Select_GetIniString(ref string NomModule, ref string MotCle, ref string FichierIni)
        {
            string returnedString = new string(' ', 640);
            string defautStr = "";
            GetPrivateProfileString(ref NomModule, ref MotCle, ref defautStr, ref returnedString, 640, ref FichierIni);
            return returnedString.Substring(1, returnedString.Trim().Length - 1);
        }

        public void Select_WriteIniString(string NomModule, string MotClé, string Chaine, string FichierIni)
        {
            WritePrivateProfileString(ref NomModule, ref MotClé, ref Chaine, ref FichierIni);
        }

        private void Start_Click(object sender, EventArgs e)
        {
            if (File.Exists(this.szFileForceReStart))
            {
                this.refresh_status();
            }
            else if (MessageBox.Show("Etes-vous s\x00fbr de vouloir lancer le traitement ?", "Status Affinage Auto", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) == DialogResult.Yes)
            {
                IPrincipal principal = Thread.CurrentPrincipal;
                IIdentity identity = principal == null ? null : principal.Identity;
                string name = identity == null ? "" : identity.Name;

                File.WriteAllText(this.szFileForceReStart, name, Encoding.UTF8);
                //File.WriteAllText(this.szFileForceReStart, MyProject.User.Name, Encoding.UTF8);
                this.refresh_status();
            }
        }

        [DllImport("kernel32", EntryPoint = "WritePrivateProfileStringA", CharSet = CharSet.Ansi, SetLastError = true, ExactSpelling = true)]
        private static extern short WritePrivateProfileString([MarshalAs(UnmanagedType.VBByRefStr)] ref string MyApplicationName, [MarshalAs(UnmanagedType.VBByRefStr)] ref string MyKeyName, [MarshalAs(UnmanagedType.VBByRefStr)] ref string MykeyString, [MarshalAs(UnmanagedType.VBByRefStr)] ref string MyFileName);

        
    }
}



