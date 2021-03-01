using Kantar.MediaPlanning.Logger;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Kantar.MediaPlanning.AffinageAutoStartTool
{
    public partial class Form1 : Form
    {

        #region Global members
        public string ConsoPath { get; set; }
        public string AffinageAutoPath { get; set; }
        public string FileForceReStart { get; set; }
        public string UserFilePath { get; set; }
        public string BaseConsoFilePath { get; set; }

        #endregion

        #region Constructor
        public Form1()
        {
            InitializeComponent();
			this.Icon = Properties.Resources.logokantar;
			loadConfigfile();
		}
        #endregion

        #region Private Methods
        private void refresh_status()
		{
			TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Verbose, "refresh_status -> start");
			Show();
			this.buttonStart.Enabled = false;
			this.buttonRefresh.Enabled = false;
			if (File.Exists(FileForceReStart))
			{
				TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Verbose, "refresh_status -> file exists :"+ FileForceReStart);
				this.labelStatusValue.Text = "En attente de traitement...";
				Application.DoEvents();
				while (File.Exists(FileForceReStart))
				{
					Application.DoEvents();
					Thread.Sleep(500);
				}
				while (!File.Exists(UserFilePath))
				{
					Application.DoEvents();
					Thread.Sleep(500);
				}
			}
			else
            {
				TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Verbose, "refresh_status -> file not exists :" + FileForceReStart);
			}
			if (File.Exists(UserFilePath))
			{
				TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Verbose, "refresh_status -> file exist :" + UserFilePath);
				try
				{
					this.labelValueUser.Text = File.ReadAllLines(UserFilePath, Encoding.UTF8).First();
				}
				catch(Exception ex)
                {
					TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Error, "refresh_status -> ReadAllLines error :" + ex.Message);
				}
				this.labelValueDate.Text = File.GetLastWriteTime(UserFilePath).ToString();
				this.labelStatusValue.Text = "En cours de traitement...";
				while (File.Exists(UserFilePath))
				{
					try
					{
						DateTime lastWriteTime = File.GetLastWriteTime(UserFilePath);
						//string strB = Conversion(DateTime.DateDiff(DateInterval.Second, lastWriteTime, DateTime.Now));
						var diff = ((TimeSpan)(DateTime.Now - lastWriteTime));
						//string strB = Conversion(diff);
						if (string.Compare(this.buttonRefresh.Text, diff.ToString(@"hh\:mm\:ss")) != 0)
						{
							this.buttonRefresh.Text = diff.ToString(@"hh\:mm\:ss");
						}
					}
					catch (Exception ex)
					{
						TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Error, "refresh_status error -> lastWriteTime" + ex.Message);
					}
					Application.DoEvents();
					Thread.Sleep(100);
				}
				this.buttonRefresh.Text = "Rafraichir";
			}
			else
			{
				TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Verbose, "refresh_status -> file not exists :" + UserFilePath);
			}
			this.labelValueUser.Text = "Aucun";
			try
			{
				this.labelValueDate.Text = File.GetLastWriteTime(BaseConsoFilePath).ToString();
				this.labelStatusValue.Text = "Traitement terminé.";
			}
			catch(Exception ex)
            {
				TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Error, "BaseConsoFilePath :" + BaseConsoFilePath);
				TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Error, "BaseConsoFilePath :" + ex.Message);
				this.labelStatusValue.Text = "Erreur.";
			}
			this.buttonStart.Enabled = true;
			this.buttonRefresh.Enabled = true;
		}
		private string Conversion(double TempsTotal)
		{
			TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Error, "Conversion :" + TempsTotal);
			if (TempsTotal > 86400)
			{
				return "00:00:00";
			}
			else
			{
				TimeSpan time = TimeSpan.FromSeconds(TempsTotal);
				return time.ToString(@"hh\:mm\:ss");
			}
		}
		private void loadConfigfile()
		{
			TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Verbose, "LoadConfigfile -> start");
			try
			{
				ConsoPath = ConfigurationManager.AppSettings["TargetConsoPath"];
				AffinageAutoPath = ConfigurationManager.AppSettings["TargetAffinageAutoPath"];
				UserFilePath = ConfigurationManager.AppSettings["UserFile"];
				BaseConsoFilePath = ConfigurationManager.AppSettings["BaseConsoFile"];
				FileForceReStart = ConfigurationManager.AppSettings["FileForceReStart"];
				FileForceReStart = Path.Combine(AffinageAutoPath, FileForceReStart);
				UserFilePath = Path.Combine(ConsoPath, UserFilePath);
				BaseConsoFilePath = Path.Combine(ConsoPath, BaseConsoFilePath);
			}
			catch (Exception ex)
			{
				TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Error, "LoadConfigfile error :" + ex.Message);
				this.labelStatus.Text = "Erreur chargement config";
			}
		}
		#endregion

		#region Events

		private void buttonRefresh_Click(object sender, EventArgs e)
        {
            refresh_status();
        }

        private void buttonStart_Click(object sender, EventArgs e)
        {
			TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Verbose, "buttonStart_Click -> start");

			if (File.Exists(FileForceReStart))
			{
				TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Verbose, "buttonStart_Click -> file exists :"+ FileForceReStart);
				refresh_status();
			}
			else if (MessageBox.Show("Etes-vous sûr de vouloir lancer le traitement ?", "Status Affinage Auto", MessageBoxButtons.YesNo, MessageBoxIcon.Exclamation) == DialogResult.Yes)
			{
				TraceLoggingtool.WriteLineIf(System.Diagnostics.TraceLevel.Verbose, "buttonStart_Click -> Msgbox Yes");
				//File.WriteAllText(FileForceReStart, MyProject.User.Name, Encoding.UTF8);
				File.WriteAllText(FileForceReStart, System.Security.Principal.WindowsIdentity.GetCurrent().Name, Encoding.UTF8);
				refresh_status();
			}
		}

        private void buttonExit_Click(object sender, EventArgs e)
        {
            Close();
        }

		private void Form1_Load(object sender, EventArgs e)
		{
			refresh_status();
		}

		#endregion


	}
}
