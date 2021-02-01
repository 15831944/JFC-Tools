// MultiUserEDI.MultiUserEDI
using IWshRuntimeLibrary;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.CompilerServices;
using MultiUserEDI.My;
using MultiUserEDI.Properties;
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using CompareMethod = Microsoft.VisualBasic.CompareMethod;
using File = System.IO.File;

namespace MultiUserEDI
{
    internal class MultiUserEDI : Form
    {
        private IContainer components;

        [CompilerGenerated]
        [AccessedThroughProperty("ListUsers")]
        private ListBox _ListUsers;

        [CompilerGenerated]
        [AccessedThroughProperty("Button_Launch")]
        private Button _Button_Launch;

        [CompilerGenerated]
        [AccessedThroughProperty("Button_Quitter")]
        private Button _Button_Quitter;

        [CompilerGenerated]
        [AccessedThroughProperty("LabelVeuillez")]
        private Label _LabelVeuillez;

        [CompilerGenerated]
        [AccessedThroughProperty("GroupBoxEDI")]
        private GroupBox _GroupBoxEDI;

        [CompilerGenerated]
        [AccessedThroughProperty("RadioButton_UsersAssets")]
        private RadioButton _RadioButton_UsersAssets;

        [CompilerGenerated]
        [AccessedThroughProperty("RadioButton_AllCompany")]
        private RadioButton _RadioButton_AllCompany;

        [CompilerGenerated]
        [AccessedThroughProperty("RadioButton_FolderEDI")]
        private RadioButton _RadioButton_FolderEDI;

        [CompilerGenerated]
        [AccessedThroughProperty("RadioButton_Nothing")]
        private RadioButton _RadioButton_Nothing;

        [CompilerGenerated]
        [AccessedThroughProperty("CheckedListBox_UsersEDI")]
        private CheckedListBox _CheckedListBox_UsersEDI;

        [CompilerGenerated]
        [AccessedThroughProperty("Button_Valid")]
        private Button _Button_Valid;

        [CompilerGenerated]
        [AccessedThroughProperty("Button_Cancel")]
        private Button _Button_Cancel;

        [CompilerGenerated]
        [AccessedThroughProperty("Button_Param")]
        private Button _Button_Param;

        [CompilerGenerated]
        [AccessedThroughProperty("RadioButton_AssetsEDI")]
        private RadioButton _RadioButton_AssetsEDI;

        [CompilerGenerated]
        [AccessedThroughProperty("Label_Version")]
        private Label _Label_Version;

        private readonly DateTime DIndex;

        private readonly DateTime MIndex;

        public int ReturnCodeError;

        public int ReturnCodeLicense;

        private readonly long interval;

        private readonly long interval_license;

        [CompilerGenerated]
        [AccessedThroughProperty("Connection")]
        private CConnection _Connection;

        private const int BEFORE = -1;

        private const int AFTER = 1;

        private const int APPEND = 2;

        private const int REMPLACE = 0;

        private const int CONTINU = 1;

        private const int RESTART = 0;

        private const int END_OF_FILE = -4;

        private const int FILE_NOT_FOUND = -2;

        private const int FILE_LINE_LENGTH = -3;

        private const int OTHER_FAILURE = -1;

        private const int FILE_RD_ONLY = -5;

        private const int LINE_NUMBER = -7;

        private const int OUT_OF_DISK_SPACE = -6;

        private const short INDEX_COMPANY_NAME = 0;

        private const short INDEX_PATH_USERS = 1;

        private const short INDEX_FOLDER_EXTERNE = 2;

        private const short INDEX_CODE_EDI = 3;

        private const short INDEX_FOLDER_NOMENCLATURE = 4;

        private const short INDEX_USER_EMAIL = 5;

        private const short INDEX_RAISON_NEW = 6;

        private const short INDEX_SOCIETE_MBS = 7;

        private const short INDEX_CONSOLIDATION = 8;

        private const short INDEX_AFFINAGE = 9;

        private const short INDEX_INTF_MBS = 10;

        private const short INDEX_LAST = 10;

        private const short NUMBER_COMPANY_MAX = 12;

        private const short NUMBER_EINTERLOC_MAX = 29;

        public bool bUseShellExecute;

        public bool bUseProcessExecute;

        public bool TableUserExist;

        public string AppPath;

        private string DefaultReadingEDI;

        private string PATH_USERS;

        private string USER_EMAIL;

        private string PATH_FILE_TABLE_USER;

        private string FOLDER_EXTERNE;

        private string LAST_COMPANY;

        private string COMPANY_NAME;

        private string EMAIL_VERIF;

        private int COMPANY_NUMBER;

        private string N_CLIENT;

        private string USER_TABLE;

        private readonly string[,] TableUtilisateur;

        private readonly ArrayList ListUnivers;

        private readonly string AppVersion;

        internal virtual ListBox ListUsers
        {
            [CompilerGenerated]
            get
            {
                return _ListUsers;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = ListUsers_DoubleClick;
                EventHandler value3 = ListUsers_SelectedIndexChanged;
                ListBox listUsers = _ListUsers;
                if (listUsers != null)
                {
                    listUsers.DoubleClick -= value2;
                    listUsers.SelectedIndexChanged -= value3;
                }
                _ListUsers = value;
                listUsers = _ListUsers;
                if (listUsers != null)
                {
                    listUsers.DoubleClick += value2;
                    listUsers.SelectedIndexChanged += value3;
                }
            }
        }

        internal virtual Button Button_Launch
        {
            [CompilerGenerated]
            get
            {
                return _Button_Launch;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = Button_Launch_Click;
                Button button_Launch = _Button_Launch;
                if (button_Launch != null)
                {
                    button_Launch.Click -= value2;
                }
                _Button_Launch = value;
                button_Launch = _Button_Launch;
                if (button_Launch != null)
                {
                    button_Launch.Click += value2;
                }
            }
        }

        internal virtual Button Button_Quitter
        {
            [CompilerGenerated]
            get
            {
                return _Button_Quitter;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = Quitter_Click;
                Button button_Quitter = _Button_Quitter;
                if (button_Quitter != null)
                {
                    button_Quitter.Click -= value2;
                }
                _Button_Quitter = value;
                button_Quitter = _Button_Quitter;
                if (button_Quitter != null)
                {
                    button_Quitter.Click += value2;
                }
            }
        }

        [field: AccessedThroughProperty("LabelVeuillez")]
        internal virtual Label LabelVeuillez
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("GroupBoxEDI")]
        internal virtual GroupBox GroupBoxEDI
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("RadioButton_UsersAssets")]
        internal virtual RadioButton RadioButton_UsersAssets
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("RadioButton_AllCompany")]
        internal virtual RadioButton RadioButton_AllCompany
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("RadioButton_FolderEDI")]
        internal virtual RadioButton RadioButton_FolderEDI
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("RadioButton_Nothing")]
        internal virtual RadioButton RadioButton_Nothing
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual CheckedListBox CheckedListBox_UsersEDI
        {
            [CompilerGenerated]
            get
            {
                return _CheckedListBox_UsersEDI;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ItemCheckEventHandler value2 = CheckedListBox_UsersEDI_ItemCheck;
                EventHandler value3 = CheckedListBox_UsersEDI_DoubleClick;
                CheckedListBox checkedListBox_UsersEDI = _CheckedListBox_UsersEDI;
                if (checkedListBox_UsersEDI != null)
                {
                    checkedListBox_UsersEDI.ItemCheck -= value2;
                    checkedListBox_UsersEDI.DoubleClick -= value3;
                }
                _CheckedListBox_UsersEDI = value;
                checkedListBox_UsersEDI = _CheckedListBox_UsersEDI;
                if (checkedListBox_UsersEDI != null)
                {
                    checkedListBox_UsersEDI.ItemCheck += value2;
                    checkedListBox_UsersEDI.DoubleClick += value3;
                }
            }
        }

        internal virtual Button Button_Valid
        {
            [CompilerGenerated]
            get
            {
                return _Button_Valid;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = Button_Valid_Click;
                Button button_Valid = _Button_Valid;
                if (button_Valid != null)
                {
                    button_Valid.Click -= value2;
                }
                _Button_Valid = value;
                button_Valid = _Button_Valid;
                if (button_Valid != null)
                {
                    button_Valid.Click += value2;
                }
            }
        }

        internal virtual Button Button_Cancel
        {
            [CompilerGenerated]
            get
            {
                return _Button_Cancel;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = Button_Cancel_Click;
                Button button_Cancel = _Button_Cancel;
                if (button_Cancel != null)
                {
                    button_Cancel.Click -= value2;
                }
                _Button_Cancel = value;
                button_Cancel = _Button_Cancel;
                if (button_Cancel != null)
                {
                    button_Cancel.Click += value2;
                }
            }
        }

        internal virtual Button Button_Param
        {
            [CompilerGenerated]
            get
            {
                return _Button_Param;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = Button_Param_Click;
                Button button_Param = _Button_Param;
                if (button_Param != null)
                {
                    button_Param.Click -= value2;
                }
                _Button_Param = value;
                button_Param = _Button_Param;
                if (button_Param != null)
                {
                    button_Param.Click += value2;
                }
            }
        }

        [field: AccessedThroughProperty("RadioButton_AssetsEDI")]
        internal virtual RadioButton RadioButton_AssetsEDI
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("Label_Version")]
        internal virtual Label Label_Version
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("Connection")]
        public virtual CConnection Connection
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        //public static bool KeyStatus
        //{
        //    get
        //    {
        //        if (Key == Keys.LButton && MyProject.Computer.Mouse.ButtonsSwapped)
        //        {
        //            Key = Keys.RButton;
        //        }
        //        else if (Key == Keys.RButton && MyProject.Computer.Mouse.ButtonsSwapped)
        //        {
        //            Key = Keys.LButton;
        //        }
        //        return (GetAsyncKeyState((int)Key) & 0x8000) != 0;
        //    }
        //}

        public static bool get_KeyStatus(Keys Key)
        {
            if ((Key == Keys.LButton) && MyProject.Computer.Mouse.ButtonsSwapped)
            {
                Key = Keys.RButton;
            }
            else if ((Key == Keys.RButton) && MyProject.Computer.Mouse.ButtonsSwapped)
            {
                Key = Keys.LButton;
            }
            return ((GetAsyncKeyState((int)Key) & 0x8000) != 0);
        }

        public MultiUserEDI()
        {
            base.FormClosing += MultiUserEDI_FormClosing;
            base.Load += MultiUserEDI_Load;
            DIndex = Conversions.ToDate("01/01/2099");
            MIndex = Conversions.ToDate("01/01/2099");
            //ReturnCodeError = Conversions.ToInteger(MyProject.Computer.Registry.GetValue("HKEY_CURRENT_USER\\Software\\MeyoSoft", "Acces", 0));
            //ReturnCodeLicense = Conversions.ToInteger(MyProject.Computer.Registry.GetValue("HKEY_CURRENT_USER\\Software\\MeyoSoft", "License", 0));
            ReturnCodeError = 0;
            ReturnCodeLicense = 0;
            interval = DateAndTime.DateDiff(DateInterval.Day, DIndex, DateTime.Now);
            interval_license = DateAndTime.DateDiff(DateInterval.Day, MIndex, DateTime.Now);
            TableUserExist = true;
            EMAIL_VERIF = null;
            COMPANY_NUMBER = 0;
            N_CLIENT = null;
            USER_TABLE = null;
            TableUtilisateur = new string[13, 11];
            ListUnivers = new ArrayList();
            AppVersion = FileVersionInfo.GetVersionInfo(Application.ExecutablePath).FileVersion;
            InitializeComponent();
        }

        [DebuggerNonUserCode]
        protected override void Dispose(bool disposing)
        {
            try
            {
                if (disposing && components != null)
                {
                    components.Dispose();
                }
            }
            finally
            {
                base.Dispose(disposing);
            }
        }

        [System.Diagnostics.DebuggerStepThrough]
        private void InitializeComponent()
        {
            ListUsers = new System.Windows.Forms.ListBox();
            LabelVeuillez = new System.Windows.Forms.Label();
            GroupBoxEDI = new System.Windows.Forms.GroupBox();
            RadioButton_AssetsEDI = new System.Windows.Forms.RadioButton();
            Button_Param = new System.Windows.Forms.Button();
            RadioButton_Nothing = new System.Windows.Forms.RadioButton();
            RadioButton_UsersAssets = new System.Windows.Forms.RadioButton();
            RadioButton_AllCompany = new System.Windows.Forms.RadioButton();
            RadioButton_FolderEDI = new System.Windows.Forms.RadioButton();
            CheckedListBox_UsersEDI = new System.Windows.Forms.CheckedListBox();
            Button_Cancel = new System.Windows.Forms.Button();
            Button_Valid = new System.Windows.Forms.Button();
            Button_Quitter = new System.Windows.Forms.Button();
            Button_Launch = new System.Windows.Forms.Button();
            Label_Version = new System.Windows.Forms.Label();
            GroupBoxEDI.SuspendLayout();
            SuspendLayout();
            ListUsers.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            ListUsers.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            ListUsers.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ListUsers.FormattingEnabled = true;
            ListUsers.IntegralHeight = false;
            ListUsers.ItemHeight = 18;
            ListUsers.Location = new System.Drawing.Point(12, 30);
            ListUsers.Name = "ListUsers";
            ListUsers.Size = new System.Drawing.Size(218, 292);
            ListUsers.TabIndex = 0;
            LabelVeuillez.AutoSize = true;
            LabelVeuillez.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            LabelVeuillez.Location = new System.Drawing.Point(12, 9);
            LabelVeuillez.Name = "LabelVeuillez";
            LabelVeuillez.Size = new System.Drawing.Size(223, 18);
            LabelVeuillez.TabIndex = 3;
            LabelVeuillez.Text = "Veuillez choisir un environement:";
            GroupBoxEDI.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right;
            GroupBoxEDI.Controls.Add(RadioButton_AssetsEDI);
            GroupBoxEDI.Controls.Add(Button_Param);
            GroupBoxEDI.Controls.Add(RadioButton_Nothing);
            GroupBoxEDI.Controls.Add(RadioButton_UsersAssets);
            GroupBoxEDI.Controls.Add(RadioButton_AllCompany);
            GroupBoxEDI.Controls.Add(RadioButton_FolderEDI);
            GroupBoxEDI.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            GroupBoxEDI.Location = new System.Drawing.Point(236, 130);
            GroupBoxEDI.Name = "GroupBoxEDI";
            GroupBoxEDI.Size = new System.Drawing.Size(139, 193);
            GroupBoxEDI.TabIndex = 10;
            GroupBoxEDI.TabStop = false;
            GroupBoxEDI.Text = "Options retour EDI:";
            RadioButton_AssetsEDI.AutoSize = true;
            RadioButton_AssetsEDI.Font = new System.Drawing.Font("Microsoft Sans Serif", 9f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            RadioButton_AssetsEDI.Location = new System.Drawing.Point(6, 121);
            RadioButton_AssetsEDI.Name = "RadioButton_AssetsEDI";
            RadioButton_AssetsEDI.Size = new System.Drawing.Size(120, 19);
            RadioButton_AssetsEDI.TabIndex = 17;
            RadioButton_AssetsEDI.TabStop = true;
            RadioButton_AssetsEDI.Text = "Actifs et réception";
            RadioButton_AssetsEDI.UseVisualStyleBackColor = true;
            Button_Param.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            Button_Param.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            Button_Param.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            //Button_Param.Image = MultiUserEDI.My.Resources.Resources.telecharger_icone_4254_32;
            Button_Param.Image = Resources.telecharger_icone_4254_32;
            Button_Param.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            Button_Param.Location = new System.Drawing.Point(6, 146);
            Button_Param.Name = "Button_Param";
            Button_Param.Size = new System.Drawing.Size(127, 41);
            Button_Param.TabIndex = 16;
            Button_Param.Text = "     Charger";
            Button_Param.UseVisualStyleBackColor = true;
            RadioButton_Nothing.AutoSize = true;
            RadioButton_Nothing.Font = new System.Drawing.Font("Microsoft Sans Serif", 9f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            RadioButton_Nothing.Location = new System.Drawing.Point(6, 21);
            RadioButton_Nothing.Name = "RadioButton_Nothing";
            RadioButton_Nothing.Size = new System.Drawing.Size(131, 19);
            RadioButton_Nothing.TabIndex = 13;
            RadioButton_Nothing.TabStop = true;
            RadioButton_Nothing.Text = "Aucun changement";
            RadioButton_Nothing.UseVisualStyleBackColor = true;
            RadioButton_UsersAssets.AutoSize = true;
            RadioButton_UsersAssets.Font = new System.Drawing.Font("Microsoft Sans Serif", 9f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            RadioButton_UsersAssets.Location = new System.Drawing.Point(6, 96);
            RadioButton_UsersAssets.Name = "RadioButton_UsersAssets";
            RadioButton_UsersAssets.Size = new System.Drawing.Size(117, 19);
            RadioButton_UsersAssets.TabIndex = 12;
            RadioButton_UsersAssets.TabStop = true;
            RadioButton_UsersAssets.Text = "Utilisateurs actifs";
            RadioButton_UsersAssets.UseVisualStyleBackColor = true;
            RadioButton_AllCompany.AutoSize = true;
            RadioButton_AllCompany.Font = new System.Drawing.Font("Microsoft Sans Serif", 9f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            RadioButton_AllCompany.Location = new System.Drawing.Point(6, 71);
            RadioButton_AllCompany.Name = "RadioButton_AllCompany";
            RadioButton_AllCompany.Size = new System.Drawing.Size(133, 19);
            RadioButton_AllCompany.TabIndex = 11;
            RadioButton_AllCompany.TabStop = true;
            RadioButton_AllCompany.Text = "Tous les utilisateurs";
            RadioButton_AllCompany.UseVisualStyleBackColor = true;
            RadioButton_FolderEDI.AutoSize = true;
            RadioButton_FolderEDI.Font = new System.Drawing.Font("Microsoft Sans Serif", 9f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            RadioButton_FolderEDI.Location = new System.Drawing.Point(6, 46);
            RadioButton_FolderEDI.Name = "RadioButton_FolderEDI";
            RadioButton_FolderEDI.Size = new System.Drawing.Size(121, 19);
            RadioButton_FolderEDI.TabIndex = 10;
            RadioButton_FolderEDI.TabStop = true;
            RadioButton_FolderEDI.Text = "Dossier réception";
            RadioButton_FolderEDI.UseVisualStyleBackColor = true;
            CheckedListBox_UsersEDI.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            CheckedListBox_UsersEDI.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            CheckedListBox_UsersEDI.CheckOnClick = true;
            CheckedListBox_UsersEDI.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            CheckedListBox_UsersEDI.FormattingEnabled = true;
            CheckedListBox_UsersEDI.IntegralHeight = false;
            CheckedListBox_UsersEDI.Location = new System.Drawing.Point(12, 30);
            CheckedListBox_UsersEDI.Name = "CheckedListBox_UsersEDI";
            CheckedListBox_UsersEDI.Size = new System.Drawing.Size(218, 292);
            CheckedListBox_UsersEDI.TabIndex = 11;
            CheckedListBox_UsersEDI.Visible = false;
            Button_Cancel.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right;
            Button_Cancel.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            Button_Cancel.Font = new System.Drawing.Font("Microsoft Sans Serif", 12f, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            Button_Cancel.Image = Resources.fermer_gtk_icone_6139_32;
            Button_Cancel.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            Button_Cancel.Location = new System.Drawing.Point(236, 84);
            Button_Cancel.Name = "Button_Cancel";
            Button_Cancel.Size = new System.Drawing.Size(139, 40);
            Button_Cancel.TabIndex = 13;
            Button_Cancel.Text = "      &Annuler";
            Button_Cancel.UseVisualStyleBackColor = true;
            Button_Cancel.Visible = false;
            Button_Valid.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right;
            Button_Valid.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            Button_Valid.Font = new System.Drawing.Font("Microsoft Sans Serif", 12f, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            Button_Valid.Image = Resources.appliquer_verifier_ok_oui_icone_5318_32;
            Button_Valid.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            Button_Valid.Location = new System.Drawing.Point(236, 38);
            Button_Valid.Name = "Button_Valid";
            Button_Valid.Size = new System.Drawing.Size(139, 40);
            Button_Valid.TabIndex = 12;
            Button_Valid.Text = "      &Valider";
            Button_Valid.UseVisualStyleBackColor = true;
            Button_Valid.Visible = false;
            Button_Quitter.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right;
            Button_Quitter.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            Button_Quitter.Font = new System.Drawing.Font("Microsoft Sans Serif", 12f, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            Button_Quitter.Image = Resources.Button_Quitter_Image;
            Button_Quitter.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            Button_Quitter.Location = new System.Drawing.Point(236, 84);
            Button_Quitter.Name = "Button_Quitter";
            Button_Quitter.Size = new System.Drawing.Size(139, 40);
            Button_Quitter.TabIndex = 2;
            Button_Quitter.Text = "     &Quitter";
            Button_Quitter.UseVisualStyleBackColor = true;
            Button_Launch.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right;
            Button_Launch.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            Button_Launch.Font = new System.Drawing.Font("Microsoft Sans Serif", 12f, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, 0);
            Button_Launch.Image = Resources.jouer_a_droite_fleche_icone_6822_32;
            Button_Launch.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            Button_Launch.Location = new System.Drawing.Point(236, 38);
            Button_Launch.Name = "Button_Launch";
            Button_Launch.Size = new System.Drawing.Size(139, 40);
            Button_Launch.TabIndex = 1;
            Button_Launch.Text = "     &Exécuter";
            Button_Launch.UseVisualStyleBackColor = true;
            Label_Version.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right;
            Label_Version.AutoSize = true;
            Label_Version.Font = new System.Drawing.Font("Microsoft Sans Serif", 6f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            Label_Version.Location = new System.Drawing.Point(348, 9);
            Label_Version.Name = "Label_Version";
            Label_Version.Size = new System.Drawing.Size(27, 9);
            Label_Version.TabIndex = 18;
            Label_Version.Text = "1.0.0.0";
            base.AutoScaleDimensions = new System.Drawing.SizeF(6f, 13f);
            base.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            base.ClientSize = new System.Drawing.Size(387, 334);
            base.Controls.Add(Label_Version);
            base.Controls.Add(Button_Cancel);
            base.Controls.Add(Button_Valid);
            base.Controls.Add(CheckedListBox_UsersEDI);
            base.Controls.Add(GroupBoxEDI);
            base.Controls.Add(LabelVeuillez);
            base.Controls.Add(Button_Quitter);
            base.Controls.Add(Button_Launch);
            base.Controls.Add(ListUsers);
            //base.Icon = (System.Drawing.Icon)resources.GetObject("$this.Icon");
            base.Icon = Resources.Icon;
            MinimumSize = new System.Drawing.Size(400, 200);
            base.Name = "MultiUserEDI";
            Text = "Atelier Radio";
            GroupBoxEDI.ResumeLayout(false);
            GroupBoxEDI.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        [DllImport("user32.dll", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
        public static extern int GetAsyncKeyState(int vKey);

        private void Update_MultiUser()
        {
            string path = mFileFunction.ParsePath(Application.ExecutablePath, 8) + ".old";
            if (!File.Exists(path))
            {
                return;
            }
            MyProject.Forms.PleaseWait.Show();
            Application.DoEvents();
            int num = 0;
            do
            {
                try
                {
                    File.Delete(path);
                    MyProject.Forms.PleaseWait.Hide();
                    return;
                }
                catch (Exception ex)
                {
                    ProjectData.SetProjectError(ex);
                    Exception ex2 = ex;
                    ProjectData.ClearProjectError();
                }
                Thread.Sleep(100);
                Application.DoEvents();
                num = checked(num + 1);
            }
            while (num <= 100);
            MyProject.Forms.PleaseWait.Hide();
        }

        private void ButtonQuitter_Click()
        {
            Close();
        }

        private static bool BaliseConsoData(string p)
        {
            if (p.Contains("<p name=\"consoData"))
            {
                return true;
            }
            return false;
        }

        private static bool BaliseConfig(string p)
        {
            if (p.Contains("<config>"))
            {
                return true;
            }
            return false;
        }

        private static bool TiersPO(string p)
        {
            if (p.Contains("\"tiers\";\"PO\""))
            {
                return true;
            }
            return false;
        }

        private static bool Campagne(string p)
        {
            string text = "\"Campagne\";\"";
            if (Strings.StrComp(p.Substring(0, Strings.Len(text)), text, CompareMethod.Text) == 0)
            {
                return true;
            }
            return false;
        }

        private static bool TiersZDS(string p)
        {
            if (p.Contains("\"tiers\";\"ZDS\""))
            {
                return true;
            }
            return false;
        }

        private static bool TiersDS(string p)
        {
            if (p.Contains("\"tiers\";\"DS\""))
            {
                return true;
            }
            return false;
        }

        private string ExtractPaht(string s)
        {
            string text = "path=\"";
            int num = Strings.InStr(s, text, CompareMethod.Text);
            checked
            {
                if (num > 0)
                {
                    int num2 = Strings.InStr(num + Strings.Len(text), s, "\"", CompareMethod.Text);
                    return Strings.Mid(s, num + Strings.Len(text), num2 - num - Strings.Len(text));
                }
                return null;
            }
        }

        private void BugTrap(string svSRCDIR)
        {
            if (!Directory.Exists(svSRCDIR))
            {
                return;
            }
            if (Operators.CompareString(Strings.Right(svSRCDIR, 1), "\\", TextCompare: false) != 0)
            {
                svSRCDIR += "\\";
            }
            string text = null;
            try
            {
                text = "5x00";
                string NomModule = "Localisation";
                string MotCle = "RAPPORTSERREURS";
                string FichierIni = AppPath + "Ataaetud.ini";
                string text2 = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref FichierIni);
                text = "5x01";
                if (Operators.CompareString(text2, null, TextCompare: false) == 0)
                {
                    return;
                }
                if (Operators.CompareString(Strings.Right(text2, 1), "\\", TextCompare: false) != 0)
                {
                    text2 += "\\";
                }
                text = "5x02";
                if (!Directory.Exists(text2))
                {
                    text = "5x03";
                    Directory.CreateDirectory(text2);
                    text = "5x04";
                }
                text = "5x05";
                string[] files = Directory.GetFiles(svSRCDIR, "Creuset_error_report_*.zip", SearchOption.TopDirectoryOnly);
                text = "5x06";
                if (files.Length <= 0)
                {
                    return;
                }
                text = "5x07";
                string[] array = files;
                foreach (string text3 in array)
                {
                    string text4 = text3.Replace(svSRCDIR, text2);
                    int num = 1;
                    text = "5x07-0" + Conversions.ToString(num);
                    while (File.Exists(text4))
                    {
                        text4 = text3.Replace(svSRCDIR, text2).Replace(".zip", "(" + num + ").zip");
                        num = checked(num + 1);
                        text = "5x07-0" + Conversions.ToString(num);
                    }
                    text = "5x08" + Conversions.ToString(num);
                    File.Move(text3, text4);
                }
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                Interaction.MsgBox(ex2.Message + "(" + text + ")", MsgBoxStyle.Critical);
                ProjectData.ClearProjectError();
            }
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        private void CompanySelected()
        {
            if ((interval_license > 0) | (ReturnCodeLicense == 3))
            {
                if (ReturnCodeLicense != 3)
                {
                    ReturnCodeLicense = 3;
                    MyProject.Computer.Registry.SetValue("HKEY_CURRENT_USER\\Software\\MeyoSoft", "License", ReturnCodeLicense);
                }
                Interaction.MsgBox("Your license expired. You must renew your license in order to run this software. Contact your vendor to renew your license.", MsgBoxStyle.Critical, MyProject.Application.Info.Title);
                base.Visible = false;
                Close();
            }
            Hide();
            bool @checked = RadioButton_AllCompany.Checked;
            bool flag = RadioButton_AllCompany.Checked | RadioButton_UsersAssets.Checked;
            bool checked2 = RadioButton_FolderEDI.Checked;
            bool checked3 = RadioButton_AssetsEDI.Checked;
            bool checked4 = RadioButton_Nothing.Checked;
            COMPANY_NAME = TableUtilisateur[ListUsers.SelectedIndex, 0];
            PATH_USERS = TableUtilisateur[ListUsers.SelectedIndex, 1];
            FOLDER_EXTERNE = TableUtilisateur[ListUsers.SelectedIndex, 2];
            string text = TableUtilisateur[ListUsers.SelectedIndex, 3];
            string text2 = TableUtilisateur[ListUsers.SelectedIndex, 4];
            USER_EMAIL = TableUtilisateur[ListUsers.SelectedIndex, 5];
            string text3 = TableUtilisateur[ListUsers.SelectedIndex, 6];
            string text4 = TableUtilisateur[ListUsers.SelectedIndex, 7];
            string text5 = TableUtilisateur[ListUsers.SelectedIndex, 8];
            string text6 = TableUtilisateur[ListUsers.SelectedIndex, 9];
            string text7 = TableUtilisateur[ListUsers.SelectedIndex, 10];
            IEnumerator enumerator = default(IEnumerator);
            try
            {
                enumerator = ListUnivers.GetEnumerator();
                while (enumerator.MoveNext())
                {
                    string fichierIni = Conversions.ToString(enumerator.Current);
                    if (Operators.CompareString(FOLDER_EXTERNE, null, TextCompare: false) != 0)
                    {
                        mFileIni.Select_WriteIniString("SAS", "Repertoire SE", FOLDER_EXTERNE, fichierIni);
                        mFileIni.Select_WriteIniString("SAS", "Sas Campagne", "0,0,146,328,100,\"Campagnes\"", fichierIni);
                        mFileIni.Select_WriteIniString("SAS", "Sas Externe", "0,0,246,328,100,\"Externe\"", fichierIni);
                        mFileIni.Select_WriteIniString("SAS", "Timer SE", "60", fichierIni);
                        mFileIni.Select_WriteIniString("SAS", "Nom DLL SE", "atrjfedi.dll", fichierIni);
                        mFileIni.Select_WriteIniString("SAS", "Type fichier SE", "*.txt", fichierIni);
                    }
                    else
                    {
                        mFileIni.Select_WriteIniString("SAS", "Repertoire SE", "", fichierIni);
                        mFileIni.Select_WriteIniString("SAS", "Sas Campagne", "", fichierIni);
                        mFileIni.Select_WriteIniString("SAS", "Sas Externe", "", fichierIni);
                        mFileIni.Select_WriteIniString("SAS", "Timer SE", "", fichierIni);
                        mFileIni.Select_WriteIniString("SAS", "Nom DLL SE", "", fichierIni);
                        mFileIni.Select_WriteIniString("SAS", "Type fichier SE", "", fichierIni);
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
            mFileIni.Select_WriteIniString("Definition_du_site", "EInterloc", USER_EMAIL, AppPath + "Ataaetud.ini");
            mFileIni.Select_WriteIniString("Localisation", "USER", PATH_USERS, AppPath + "Ataaetud.ini");
            mFileIni.Select_WriteIniString("DefOption", "LastCompany", COMPANY_NAME, AppPath + "Ataaetud.ini");
            if (Operators.CompareString(text4, "V7", TextCompare: false) != 0)
            {
                if (Operators.CompareString(text4, null, TextCompare: false) != 0)
                {
                    mFileIni.Select_WriteIniString("DefOption", "SOCIETE_MBS", text4, AppPath + "Ataaetud.ini");
                }
                else
                {
                    mFileIni.Select_WriteIniString("DefOption", "SOCIETE_MBS", "", AppPath + "Ataaetud.ini");
                }
            }
            if (Operators.CompareString(FOLDER_EXTERNE, null, TextCompare: false) != 0)
            {
                mFileIni.Select_WriteIniString("Localisation", "EXTERNE", FOLDER_EXTERNE, AppPath + "Ataaetud.ini");
                mFileIni.Select_WriteIniString("Definition_du_site", "Genre_Poste_Travail", "3", AppPath + "Ataaetud.ini");
            }
            else
            {
                mFileIni.Select_WriteIniString("Localisation", "EXTERNE", "", AppPath + "Ataaetud.ini");
                mFileIni.Select_WriteIniString("Definition_du_site", "Genre_Poste_Travail", "0", AppPath + "Ataaetud.ini");
            }
            if (Operators.CompareString(text, null, TextCompare: false) != 0)
            {
                mFileIni.Select_WriteIniString("Definition_du_site", "Numéro_Guichet", text, AppPath + "Ataaetud.ini");
            }
            else
            {
                mFileIni.Select_WriteIniString("Definition_du_site", "Numéro_Guichet", "", AppPath + "Ataaetud.ini");
            }
            if (Operators.CompareString(text7, "V7", TextCompare: false) != 0)
            {
                if (Operators.CompareString(text7, null, TextCompare: false) != 0)
                {
                    mFileIni.Select_WriteIniString("Localisation", "INTF_MBS", text7, AppPath + "Ataaetud.ini");
                }
                else
                {
                    mFileIni.Select_WriteIniString("Localisation", "INTF_MBS", "", AppPath + "Ataaetud.ini");
                }
            }
            checked
            {
                if (Operators.CompareString(text6, "V7", TextCompare: false) != 0)
                {
                    if (Operators.CompareString(text6, null, TextCompare: false) != 0)
                    {
                        mFileIni.Select_WriteIniString("Install", "PathDataConso", text6, AppPath + "ATPIGE.ini");
                        if (File.Exists(AppPath + "AffConso.ini"))
                        {
                            mFileIni.Select_WriteIniString("Path", "PathData", text6, AppPath + "AffConso.ini");
                            mFileIni.Select_WriteIniString("Path", "PathAPF", text6, AppPath + "AffConso.ini");
                            mFileIni.Select_WriteIniString("Path", "PathDataConso", text6, AppPath + "AffConso.ini");
                        }
                        if (File.Exists(AppPath + "AffConso\\config.xml"))
                        {
                            string[] array = File.ReadAllLines(AppPath + "AffConso\\config.xml", Encoding.Default);
                            int num = Array.FindIndex(array, BaliseConsoData);
                            if (num >= 0)
                            {
                                string text8 = array[num];
                                if (Strings.StrComp(ExtractPaht(text8), text6, CompareMethod.Text) != 0)
                                {
                                    array[num] = text8.Replace(ExtractPaht(text8), text6);
                                    File.WriteAllLines(AppPath + "AffConso\\config.xml", array, Encoding.Default);
                                }
                            }
                        }
                        if (File.Exists(AppPath + "Company_access\\config.xml"))
                        {
                            string[] array2 = File.ReadAllLines(AppPath + "Company_access\\config.xml", Encoding.Default);
                            int num2 = Array.FindIndex(array2, BaliseConsoData);
                            if (num2 >= 0)
                            {
                                string text9 = array2[num2];
                                if (Strings.StrComp(ExtractPaht(text9), text6, Microsoft.VisualBasic.CompareMethod.Text) != 0)
                                {
                                    array2[num2] = text9.Replace(ExtractPaht(text9), text6);
                                    File.WriteAllLines(AppPath + "Company_access\\config.xml", array2, Encoding.Default);
                                }
                            }
                            else
                            {
                                try
                                {
                                    int num3 = Array.FindIndex(array2, BaliseConfig) + 1;
                                    string[] array3 = new string[array2.Length + 1];
                                    int num4 = array2.Length - 1;
                                    for (int i = 0; i <= num4; i++)
                                    {
                                        if (i == num3)
                                        {
                                            array3[i] = "\t<p name=\"consoData\" path=\"" + text6 + "\"/>";
                                        }
                                        array3[Conversions.ToInteger(Interaction.IIf(i >= num3, i + 1, i))] = array2[i];
                                    }
                                    File.WriteAllLines(AppPath + "Company_access\\config.xml", array3, Encoding.Default);
                                }
                                catch (Exception ex)
                                {
                                    ProjectData.SetProjectError(ex);
                                    Exception ex2 = ex;
                                    ProjectData.ClearProjectError();
                                }
                            }
                        }
                    }
                    else
                    {
                        mFileIni.Select_WriteIniString("Install", "PathDataConso", "", AppPath + "ATPIGE.ini");
                        if (File.Exists(AppPath + "AffConso.ini"))
                        {
                            mFileIni.Select_WriteIniString("Path", "PathData", "", AppPath + "AffConso.ini");
                            mFileIni.Select_WriteIniString("Path", "PathAPF", "", AppPath + "AffConso.ini");
                            mFileIni.Select_WriteIniString("Path", "PathDataConso", "", AppPath + "AffConso.ini");
                        }
                    }
                }
                if (Operators.CompareString(text5, "V7", TextCompare: false) != 0)
                {
                    if (Operators.CompareString(text5, null, TextCompare: false) != 0)
                    {
                        mFileIni.Select_WriteIniString("Localisation", "CONSOLIDATION", text5, AppPath + "Ataaetud.ini");
                        if (File.Exists(AppPath + "AffConso.ini"))
                        {
                            mFileIni.Select_WriteIniString("Path", "PathImport", text5, AppPath + "AffConso.ini");
                        }
                    }
                    else
                    {
                        mFileIni.Select_WriteIniString("Localisation", "CONSOLIDATION", "", AppPath + "Ataaetud.ini");
                        if (File.Exists(AppPath + "AffConso.ini"))
                        {
                            mFileIni.Select_WriteIniString("Path", "PathImport", "", AppPath + "AffConso.ini");
                        }
                    }
                }
                if (Operators.CompareString(text2, null, TextCompare: false) != 0)
                {
                    MyProject.Computer.Registry.SetValue("HKEY_CURRENT_USER\\Software\\JFC\\Nomenclature\\Configuration", "DataPath", text2);
                    if (File.Exists(AppPath + "Nomencl.exe"))
                    {
                        if (MyProject.Computer.Registry.GetValue("HKEY_CURRENT_USER\\Software\\JFC\\Nomenclature\\Configuration", "DataAcces", null) == null)
                        {
                            MyProject.Computer.Registry.SetValue("HKEY_CURRENT_USER\\Software\\JFC\\Nomenclature\\Configuration", "DataAcces", "2");
                        }
                        if (MyProject.Computer.Registry.GetValue("HKEY_CURRENT_USER\\Software\\JFC\\Nomenclature\\Configuration", "EDIAcces", null) == null)
                        {
                            MyProject.Computer.Registry.SetValue("HKEY_CURRENT_USER\\Software\\JFC\\Nomenclature\\Configuration", "EDIAcces", "2");
                        }
                        if (MyProject.Computer.Registry.GetValue("HKEY_CURRENT_USER\\Software\\JFC\\Nomenclature\\Configuration", "NomenclPath", null) == null)
                        {
                            MyProject.Computer.Registry.SetValue("HKEY_CURRENT_USER\\Software\\JFC\\Nomenclature\\Configuration", "NomenclPath", AppPath);
                        }
                        if (MyProject.Computer.Registry.GetValue("HKEY_CURRENT_USER\\Software\\JFC\\Nomenclature\\Configuration", "UserName", null) == null)
                        {
                            MyProject.Computer.Registry.SetValue("HKEY_CURRENT_USER\\Software\\JFC\\Nomenclature\\Configuration", "UserName", "UserName");
                        }
                        string folderPath = Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory);
                        if (!File.Exists(folderPath + "\\Nomenclature.lnk"))
                        {
                            IWshShortcut obj = (IWshShortcut)((WshShell)Activator.CreateInstance(Marshal.GetTypeFromCLSID(new Guid("72C24DD5-D70A-438B-8A42-98424B88AFB8")))).CreateShortcut(folderPath + "\\Nomenclature.lnk");
                            obj.TargetPath = AppPath + "Nomencl.exe";
                            obj.Save();
                        }
                    }
                    ChangeCompanyOfCtachat(text2, COMPANY_NAME, USER_EMAIL);
                    mFileIni.Select_WriteIniString("DefOption", "Nomenclature", "1", AppPath + "Ataaetud.ini");
                }
                else
                {
                    MyProject.Computer.Registry.SetValue("HKEY_CURRENT_USER\\Software\\JFC\\Nomenclature\\Configuration", "DataPath", "");
                    mFileIni.Select_WriteIniString("DefOption", "Nomenclature", "", AppPath + "Ataaetud.ini");
                }
                if (Operators.CompareString(text3, null, TextCompare: false) != 0)
                {
                    if (File.Exists(AppPath + "RAISON.NEW"))
                    {
                        File.Delete(AppPath + "RAISON.NEW");
                    }
                    try
                    {
                        File.Copy(AppPath + text3, AppPath + "RAISON.NEW");
                    }
                    catch (Exception ex3)
                    {
                        ProjectData.SetProjectError(ex3);
                        Exception ex4 = ex3;
                        Interaction.MsgBox(ex4.Message, MsgBoxStyle.Critical);
                        ProjectData.ClearProjectError();
                    }
                }
                int num5 = 1;
                ArrayList arrayList = new ArrayList();
                if (Operators.CompareString(FOLDER_EXTERNE, null, TextCompare: false) != 0)
                {
                    if (unchecked(flag || checked3))
                    {
                        if (@checked)
                        {
                            if (Strings.StrComp(DefaultReadingEDI, "AllUsers", CompareMethod.Text) != 0)
                            {
                                mFileIni.Select_WriteIniString("DefOption", "DefaultReadingEDI", "AllUsers", AppPath + "Ataaetud.ini");
                            }
                        }
                        else if (checked3)
                        {
                            if (Strings.StrComp(DefaultReadingEDI, "OnlyUsersEx", CompareMethod.Text) != 0)
                            {
                                mFileIni.Select_WriteIniString("DefOption", "DefaultReadingEDI", "OnlyUsersEx", AppPath + "Ataaetud.ini");
                            }
                        }
                        else if (Strings.StrComp(DefaultReadingEDI, "OnlyUsers", CompareMethod.Text) != 0)
                        {
                            mFileIni.Select_WriteIniString("DefOption", "DefaultReadingEDI", "OnlyUsers", AppPath + "Ataaetud.ini");
                        }
                        string[] array4 = (from x in Directory.GetFiles(Path.GetDirectoryName(PATH_FILE_TABLE_USER), "TableUtilisateur_*.tb?", SearchOption.TopDirectoryOnly)
                                           select (x) into x
                                           where Strings.StrComp(x, PATH_FILE_TABLE_USER, CompareMethod.Text) != 0
                                           select x).ToArray();
                        if (array4.Length > 0)
                        {
                            string[] array5 = array4;
                            for (int j = 0; j < array5.Length; j++)
                            {
                                string[] array6 = File.ReadAllLines(array5[j], Encoding.Default);
                                foreach (string text10 in array6)
                                {
                                    if (Operators.CompareString(text10.Trim(), null, TextCompare: false) == 0)
                                    {
                                        continue;
                                    }
                                    string[] array7 = Strings.Split(text10, ";");
                                    if (@checked)
                                    {
                                        if (Operators.CompareString(array7[0], "", TextCompare: false) == 0)
                                        {
                                            int num6 = Information.UBound(array7);
                                            for (int l = 1; l <= num6; l++)
                                            {
                                                array7[l - 1] = array7[l];
                                            }
                                            array7 = (string[])Utils.CopyArray(array7, new string[Information.UBound(array7) - 1 + 1]);
                                        }
                                        else
                                        {
                                            array7 = (string[])Utils.CopyArray(array7, new string[Information.UBound(array7) + 1]);
                                        }
                                    }
                                    else
                                    {
                                        array7 = (string[])Utils.CopyArray(array7, new string[Information.UBound(array7) + 1]);
                                    }
                                    if (Strings.StrComp(COMPANY_NAME, array7[0], CompareMethod.Text) != 0 || Strings.StrComp(USER_EMAIL, array7[5], CompareMethod.Text) == 0)
                                    {
                                        continue;
                                    }
                                    if (arrayList.Count > 0)
                                    {
                                        if (!arrayList.Contains(array7[5]))
                                        {
                                            arrayList.Add(array7[5]);
                                        }
                                    }
                                    else
                                    {
                                        arrayList.Add(array7[5]);
                                    }
                                    num5++;
                                    break;
                                }
                                if (num5 == 10)
                                {
                                    break;
                                }
                            }
                        }
                    }
                    if (unchecked(checked2 || checked3))
                    {
                        if (checked3)
                        {
                            if (Strings.StrComp(DefaultReadingEDI, "OnlyUsersEx", CompareMethod.Text) != 0)
                            {
                                mFileIni.Select_WriteIniString("DefOption", "DefaultReadingEDI", "OnlyUsersEx", AppPath + "Ataaetud.ini");
                            }
                        }
                        else if (Strings.StrComp(DefaultReadingEDI, "FolderEDI", CompareMethod.Text) != 0)
                        {
                            mFileIni.Select_WriteIniString("DefOption", "DefaultReadingEDI", "FolderEDI", AppPath + "Ataaetud.ini");
                        }
                        try
                        {
                            string[] array8 = (from w in Directory.GetFiles(FOLDER_EXTERNE + "\\Reception", "*.txt", SearchOption.TopDirectoryOnly)
                                               select (w) into w
                                               orderby File.GetLastWriteTime(w) descending
                                               select w).ToArray();
                            if (array8.Length > 0)
                            {
                                string[] array9 = array8;
                                for (int m = 0; m < array9.Length; m++)
                                {
                                    string text11 = Array.Find(File.ReadAllLines(array9[m], Encoding.Default), TiersPO);
                                    if (Operators.CompareString(text11, null, TextCompare: false) == 0)
                                    {
                                        continue;
                                    }
                                    string[] array10 = Strings.Split(Strings.Replace(text11, "\"", ""), ";");
                                    if (Strings.StrComp(USER_EMAIL, array10[10], CompareMethod.Text) == 0)
                                    {
                                        continue;
                                    }
                                    if (arrayList.Count > 0)
                                    {
                                        if (!arrayList.Contains(array10[10]))
                                        {
                                            arrayList.Add(array10[10]);
                                        }
                                    }
                                    else
                                    {
                                        arrayList.Add(array10[10]);
                                    }
                                }
                            }
                        }
                        catch (Exception ex5)
                        {
                            ProjectData.SetProjectError(ex5);
                            Exception ex6 = ex5;
                            Interaction.MsgBox(ex6.Message, MsgBoxStyle.Critical);
                            ProjectData.ClearProjectError();
                        }
                    }
                    if (checked4 && Operators.CompareString(DefaultReadingEDI, null, TextCompare: false) != 0)
                    {
                        mFileIni.Select_WriteIniString("DefOption", "DefaultReadingEDI", null, AppPath + "Ataaetud.ini");
                    }
                    if (arrayList.Count > 0)
                    {
                        num5 = 1;
                        IEnumerator enumerator2 = default(IEnumerator);
                        try
                        {
                            enumerator2 = arrayList.GetEnumerator();
                            while (enumerator2.MoveNext())
                            {
                                string chaine = Conversions.ToString(enumerator2.Current);
                                mFileIni.Select_WriteIniString("Definition_du_site", "EInterloc" + num5.ToString().Trim(), chaine, AppPath + "Ataaetud.ini");
                                num5++;
                                if (num5 > 29)
                                {
                                    break;
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
                        if (num5 < 12)
                        {
                            for (int n = num5; n <= 12; n++)
                            {
                                mFileIni.Select_WriteIniString("Definition_du_site", "EInterloc" + n.ToString().Trim(), null, AppPath + "Ataaetud.ini");
                            }
                        }
                    }
                }
                else
                {
                    int num7 = 1;
                    do
                    {
                        mFileIni.Select_WriteIniString("Definition_du_site", "EInterloc" + num7.ToString().Trim(), null, AppPath + "Ataaetud.ini");
                        num7++;
                    }
                    while (num7 <= 12);
                }
                RestoreSas(AppPath + "Ufr02.ini", COMPANY_NAME);
                RestoreSas(AppPath + "Ufr03.ini", COMPANY_NAME);
                RestoreSas(AppPath + "Ufr09.ini", COMPANY_NAME);
                RestoreSas(AppPath + "Ufr10.ini", COMPANY_NAME);
                RestoreSas(AppPath + "Ufr13.ini", COMPANY_NAME);
                RestoreSas(AppPath + "Ufr14.ini", COMPANY_NAME);
                RestoreSas(AppPath + "Ufr15.ini", COMPANY_NAME);
                string desktop = MyProject.Computer.FileSystem.SpecialDirectories.Desktop;
                BugTrap(desktop);
                BugTrap(AppPath);
                BugTrap(AppPath + "RAPPORTSERREURS");
                HttpSendMessage(0, COMPANY_NAME, "LaunchApp");
                mLaunchAppAndWait.LaunchAppAndWait(AppPath + "ATSAUVE.EXE");
                ProjectData.EndApp();
                Close();
            }
        }

        public bool PrevInstance()
        {
            if (Information.UBound(Process.GetProcessesByName(Process.GetCurrentProcess().ProcessName)) > 0)
            {
                return true;
            }
            return false;
        }

        private void Quitter_Click(object sender, EventArgs e)
        {
            Close();
        }

        //private void RazSas(string szfilename)
        //{
        //    if (!File.Exists(szfilename))
        //    {
        //        return;
        //    }
        //    bool flag = false;
        //    string NomModule = "SAS";
        //    string MotCle = "Objets SC";
        //    string[] array = Strings.Split(mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref szfilename), ",");
        //    MotCle = "SAS";
        //    NomModule = "Objets SE";
        //    string[] array2 = Strings.Split(mFileIni.Select_GetIniString(ref MotCle, ref NomModule, ref szfilename), ",");
        //    int num = 0;
        //    List<string> list = File.ReadAllLines(szfilename).ToList();
        //    string[] array3 = array;
        //    _Closure$__133 - 0 closure$__133 - = default(_Closure$__133 - 0);
        //    foreach (string text in array3)
        //    {
        //        closure$__133 - = new _Closure$__133 - 0(closure$__133 -);
        //        closure$__133 -.$VB$Local_szNumber = text.Trim();
        //        num = list.FindIndex(closure$__133 -._Lambda$__0);
        //        if (num > -1)
        //        {
        //            list.RemoveAt(num);
        //            flag = true;
        //        }
        //    }
        //    string[] array4 = array2;
        //    _Closure$__133 - 1 closure$__133 - 2 = default(_Closure$__133 - 1);
        //    foreach (string text2 in array4)
        //    {
        //        closure$__133 - 2 = new _Closure$__133 - 1(closure$__133 - 2);
        //        closure$__133 - 2.$VB$Local_szNumber = text2.Trim();
        //        num = list.FindIndex(closure$__133 - 2._Lambda$__1);
        //        if (num > -1)
        //        {
        //            list.RemoveAt(num);
        //            flag = true;
        //        }
        //    }
        //    num = list.FindIndex((string value) => value.Contains("Objets SC"));
        //    if (num > -1)
        //    {
        //        list.RemoveAt(num);
        //        flag = true;
        //    }
        //    num = list.FindIndex((string value) => value.Contains("Objets SE"));
        //    if (num > -1)
        //    {
        //        list.RemoveAt(num);
        //        flag = true;
        //    }
        //    num = list.FindIndex((string value) => value.Contains("[Sas Campagne]"));
        //    if (num > -1)
        //    {
        //        list.RemoveAt(num);
        //        flag = true;
        //    }
        //    num = list.FindIndex((string value) => value.Contains("[Sas Creuset]"));
        //    if (num > -1)
        //    {
        //        list.RemoveAt(num);
        //        flag = true;
        //    }
        //    if (flag)
        //    {
        //        File.WriteAllLines(szfilename, list);
        //    }
        //}

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        private void MultiUserEDI_FormClosing(object sender, FormClosingEventArgs e)
        {
            try
            {
                Connection.Disconnect();
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                ProjectData.ClearProjectError();
            }
            ProjectData.EndApp();
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        private void MultiUserEDI_Load(object sender, EventArgs e)
        {
            Connection = new CConnection();
            Connection.InitHTTP();
            bool result = Connection.OpenInternet();
            AppPath = Application.StartupPath + "\\";
            string NomModule = "DefOption";
            string MotCle = "LastCompany";
            string FichierIni = AppPath + "Ataaetud.ini";
            LAST_COMPANY = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref FichierIni);
            Update_MultiUser();
            if (Connection.GetNewUpdate(AppVersion))
            {
                HttpSendMessage(50, LAST_COMPANY, "Updating");
                Process.Start(Application.ExecutablePath, "ReStart");
                Close();
            }
            if (Operators.CompareString(Interaction.Command(), "ReStart", TextCompare: false) == 0)
            {
                HttpSendMessage(50, LAST_COMPANY, "Updated");
            }
            if (Operators.CompareString(Interaction.Command(), "EMAIL", TextCompare: false) == 0)
            {
                FindAllEmail(AppPath);
                ProjectData.EndApp();
            }
            if (Operators.CompareString(Interaction.Command(), "SOCIETE", TextCompare: false) == 0)
            {
                FindAllSociete(AppPath);
                ProjectData.EndApp();
            }
            if (Operators.CompareString(Interaction.Command(), "TEST", TextCompare: false) == 0)
            {
                ChangeCompanyOfCtachat(AppPath, "KR MEDIA SAS", "kantar.media@kantarmedia.com");
                ProjectData.EndApp();
            }
            FichierIni = "Localisation";
            MotCle = "USER_TABLE";
            NomModule = AppPath + "Ataaetud.ini";
            USER_TABLE = mFileIni.Select_GetIniString(ref FichierIni, ref MotCle, ref NomModule);
            if (Operators.CompareString(USER_TABLE, null, TextCompare: false) != 0)
            {
                mFileFunction.Add(USER_TABLE, 4, AppPath);
                if (Strings.UCase(USER_TABLE).Contains("PARAPPP01104"))
                {
                    string text = ".tbl";
                    string text2 = ".tbI";
                    string text3 = mFileFunction.ParsePath(USER_TABLE, 4);
                    string[] array = new string[8] { "BETAUX", "BOUREL", "GUICHAUX", "MERCIER", "OUSSI", "PEREY", "PINAULT", "TIBERGHIEN" };
                    foreach (string text4 in array)
                    {
                        string text5 = text3 + "TableUtilisateur_" + text4 + text;
                        if (File.Exists(text5))
                        {
                            string text6 = text3 + "TableUtilisateur_" + text4 + text2;
                            if (File.Exists(text6))
                            {
                                File.Delete(text5);
                                HttpSendMessage(50, text4, "Doublon");
                            }
                            else
                            {
                                File.Move(text5, text6);
                                HttpSendMessage(50, text4, "Invisible");
                            }
                        }
                    }
                    if (!File.Exists(USER_TABLE))
                    {
                        string @string = mFileFunction.ParsePath(USER_TABLE, 2);
                        string text7 = mFileFunction.ParsePath(USER_TABLE, 8);
                        string text8 = mFileFunction.ParsePath(USER_TABLE, 1);
                        if (Strings.LCase(text8).Contains("tableutilisateur_"))
                        {
                            text8 = text8.Replace("TableUtilisateur_", "");
                        }
                        if (Strings.StrComp(@string, text, CompareMethod.Text) == 0)
                        {
                            string text9 = text7 + text2;
                            if (File.Exists(text9))
                            {
                                USER_TABLE = text9;
                                mFileIni.Select_WriteIniString("Localisation", "USER_TABLE", USER_TABLE, AppPath + "Ataaetud.ini");
                                HttpSendMessage(50, text8, "Invisible");
                            }
                        }
                        else if (Strings.StrComp(@string, text2, CompareMethod.Text) == 0)
                        {
                            string text10 = text7 + text;
                            if (File.Exists(text10))
                            {
                                USER_TABLE = text10;
                                mFileIni.Select_WriteIniString("Localisation", "USER_TABLE", USER_TABLE, AppPath + "Ataaetud.ini");
                                HttpSendMessage(50, "TableUtilisateur", "Visible");
                            }
                        }
                    }
                }
            }
            N_CLIENT = Conversions.ToString(MyProject.Computer.Registry.GetValue("HKEY_CURRENT_USER\\Software\\JFC\\Nomenclature\\Configuration", "DataPath", null));
            SaveSas(AppPath + "Ufr02.ini", LAST_COMPANY);
            SaveSas(AppPath + "Ufr03.ini", LAST_COMPANY);
            SaveSas(AppPath + "Ufr09.ini", LAST_COMPANY);
            SaveSas(AppPath + "Ufr10.ini", LAST_COMPANY);
            SaveSas(AppPath + "Ufr13.ini", LAST_COMPANY);
            SaveSas(AppPath + "Ufr14.ini", LAST_COMPANY);
            SaveSas(AppPath + "Ufr15.ini", LAST_COMPANY);
            if (PrevInstance())
            {
                Close();
            }
            BackColor = Color.AliceBlue;
            Label_Version.Text = Application.ProductVersion.ToString();
            base.Opacity = 0.95;
            NomModule = "Definition_du_site";
            MotCle = "EInterloc";
            FichierIni = AppPath + "Ataaetud.ini";
            USER_EMAIL = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref FichierIni);
            FichierIni = "DefOption";
            MotCle = "DefaultReadingEDI";
            NomModule = AppPath + "Ataaetud.ini";
            DefaultReadingEDI = mFileIni.Select_GetIniString(ref FichierIni, ref MotCle, ref NomModule);
            NomModule = DefaultReadingEDI;
            switch (NomModule)
            {
                case "AllUsers":
                    RadioButton_AllCompany.Checked = true;
                    break;
                case "OnlyUsers":
                    RadioButton_UsersAssets.Checked = true;
                    break;
                case "FolderEDI":
                    RadioButton_FolderEDI.Checked = true;
                    break;
                case "OnlyUsersEx":
                    RadioButton_AssetsEDI.Checked = true;
                    break;
                default:
                    if (Operators.CompareString(NomModule, null, TextCompare: false) == 0)
                    {
                        RadioButton_Nothing.Checked = true;
                    }
                    break;
            }
            string FichierIni2;
            if (File.Exists(AppPath + "AU-Serv.ini"))
            {
                MotCle = "Parametres";
                FichierIni = "UseShellExecute";
                FichierIni2 = AppPath + "AU-Serv.ini";
                bUseShellExecute = Operators.CompareString(mFileIni.Select_GetIniString(ref MotCle, ref FichierIni, ref FichierIni2), "1", TextCompare: false) == 0;
                FichierIni2 = "Parametres";
                FichierIni = "UseProcessExecute";
                MotCle = AppPath + "AU-Serv.ini";
                bUseProcessExecute = Operators.CompareString(mFileIni.Select_GetIniString(ref FichierIni2, ref FichierIni, ref MotCle), "0", TextCompare: false) != 0;
            }
            else
            {
                bUseShellExecute = false;
                bUseProcessExecute = true;
            }
            AddUnivers("Ufr02.ini");
            AddUnivers("Ufr03.ini");
            AddUnivers("Ufr09.ini");
            AddUnivers("Ufr10.ini");
            AddUnivers("Ufr13.ini");
            AddUnivers("Ufr14.ini");
            AddUnivers("Ufr15.ini");
            GroupBoxEDI.Visible = false;
            MinimumSize = new Size(400, 200);
            base.Size = MinimumSize;
            ListUsers.Items.Clear();
            MotCle = "Localisation";
            FichierIni = "User_Table";
            FichierIni2 = AppPath + "Ataaetud.ini";
            PATH_FILE_TABLE_USER = mFileIni.Select_GetIniString(ref MotCle, ref FichierIni, ref FichierIni2);
            if ((Operators.CompareString(Strings.Mid(PATH_FILE_TABLE_USER, 2, 1), ":", TextCompare: false) != 0) & (Operators.CompareString(Strings.Mid(PATH_FILE_TABLE_USER, 2, 1), "\\", TextCompare: false) != 0))
            {
                PATH_FILE_TABLE_USER = AppPath + PATH_FILE_TABLE_USER;
            }
            checked
            {
                if ((Operators.CompareString(PATH_FILE_TABLE_USER, null, TextCompare: false) != 0) & File.Exists(PATH_FILE_TABLE_USER))
                {
                    string[] array2 = File.ReadAllLines(PATH_FILE_TABLE_USER, Encoding.Default);
                    foreach (string text11 in array2)
                    {
                        if (Operators.CompareString(text11.Trim(), null, TextCompare: false) != 0 && Strings.InStr(text11, ";", CompareMethod.Text) != 1)
                        {
                            string[] array3 = Strings.Split(text11, ";");
                            bool expression = array3.Length == 7;
                            array3 = (string[])Utils.CopyArray(array3, new string[11]);
                            TableUtilisateur[COMPANY_NUMBER, 0] = array3[0];
                            TableUtilisateur[COMPANY_NUMBER, 1] = array3[1];
                            TableUtilisateur[COMPANY_NUMBER, 2] = array3[2];
                            TableUtilisateur[COMPANY_NUMBER, 3] = array3[3];
                            TableUtilisateur[COMPANY_NUMBER, 4] = array3[4];
                            TableUtilisateur[COMPANY_NUMBER, 5] = array3[5];
                            TableUtilisateur[COMPANY_NUMBER, 6] = array3[6];
                            TableUtilisateur[COMPANY_NUMBER, 7] = Interaction.IIf(expression, "V7", array3[7]).ToString();
                            TableUtilisateur[COMPANY_NUMBER, 8] = Interaction.IIf(expression, "V7", array3[8]).ToString();
                            TableUtilisateur[COMPANY_NUMBER, 9] = Interaction.IIf(expression, "V7", array3[9]).ToString();
                            TableUtilisateur[COMPANY_NUMBER, 10] = Interaction.IIf(expression, "V7", array3[10]).ToString();
                            if (Operators.CompareString(array3[0], "", TextCompare: false) != 0)
                            {
                                ListUsers.Items.Add(TableUtilisateur[COMPANY_NUMBER, 0]);
                                COMPANY_NUMBER++;
                            }
                        }
                    }
                    ToolTip toolTip = new ToolTip();
                    toolTip.AutoPopDelay = 5000;
                    toolTip.InitialDelay = 1000;
                    toolTip.ReshowDelay = 500;
                    toolTip.ShowAlways = true;
                    toolTip.SetToolTip(ListUsers, "Double cliquez sur la environement voulue.");
                    Button_Launch.Visible = true;
                    ListUsers.Visible = true;
                    Application.DoEvents();
                    string text12;
                    try
                    {
                        text12 = Dns.GetHostName();
                    }
                    catch (Exception projectError)
                    {
                        ProjectData.SetProjectError(projectError);
                        text12 = MyProject.Computer.Name;
                        ProjectData.ClearProjectError();
                    }
                    if (text12.Contains("FR-PAR4-RDSH"))
                    {
                        if ((interval > 0) | (ReturnCodeError == 2))
                        {
                            if (ReturnCodeError != 2)
                            {
                                ReturnCodeError = 2;
                                MyProject.Computer.Registry.SetValue("HKEY_CURRENT_USER\\Software\\MeyoSoft", "Acces", ReturnCodeError);
                            }
                            Interaction.MsgBox("Your license expired. You must renew your license in order to run this software. Contact your vendor to renew your license.", MsgBoxStyle.Critical, MyProject.Application.Info.Title);
                            base.Visible = false;
                            Close();
                        }
                        string text13 = MyProject.User.Name;
                        int num = Strings.InStr(text13, "\\");
                        if (num > 0)
                        {
                            text13 = Strings.Mid(text13, num + 1, Strings.Len(text13) - num);
                        }
                        if (new string[11] { "genevieve.charron", "frederic.piot", "isabelle.dumontier", "celine.voisin", "guylaine.levesque", "fleur.toutain", "clemence.ridoux", "arnaud.glachet", "cyril.marican", "ouysieng.quach" , "delphine.santos"}.Contains(text13.ToLower()))
                        {
                            if (File.Exists(AppPath + "ACFRadio.exe"))
                            {
                                ListUsers.Items.Add("Consolidation");
                            }
                            if (File.Exists(AppPath + "AffConso\\ParamEnvironmentRadio.exe"))
                            {
                                ListUsers.Items.Add("Paramètres Environment Radio");
                            }
                            if (File.Exists(AppPath + "Company_access\\CompaniesAccess.exe"))
                            {
                                ListUsers.Items.Add("Companies Access");
                            }
                            if (File.Exists(AppPath + "Nomencl.exe"))
                            {
                                ListUsers.Items.Add("Nomenclature");
                            }
                        }
                    }
                    //if (text12.Contains("FR-PAR4-RDSH") | text12.Contains("WKWFR2582618") | text12.Contains("fredcptszen0002") | text12.Contains("FR-PAR1-M16949") | text12.Contains("FR-PAR1-M16949"))
                    if (text12.Contains("FR-PAR4-RDSH") | text12.Contains("WKWFR2582618") | text12.Contains("fredcptszen0002") | text12.Contains("FR-PAR1-M"))
                    {
                        string text14 = MyProject.User.Name;
                        int num2 = Strings.InStr(text14, "\\");
                        if (num2 > 0)
                        {
                            text14 = Strings.Mid(text14, num2 + 1, Strings.Len(text14) - num2);
                        }
                        if (new string[3] { "agathe.joubert", "arthur.dauphin", "npicosso" }.Contains(text14.ToLower()) && File.Exists(AppPath + "VERSION_TEST\\ATSAUVE.EXE"))
                        {
                            ListUsers.Items.Add("Atelier Radio (Test)");
                        }
                    }
                    if (ListUsers.Items.Count == 1)
                    {
                        ListUsers.SelectedIndex = 0;
                        if (!MultiUserEDI.get_KeyStatus(Keys.ControlKey))
                        {
                            CompanySelected();
                        }
                    }
                    else if (Operators.CompareString(LAST_COMPANY, null, TextCompare: false) != 0)
                    {
                        ListUsers.Text = LAST_COMPANY;
                    }
                    bool flag = Strings.InStr(Path.GetFileName(PATH_FILE_TABLE_USER), "TableUtilisateur_", CompareMethod.Text) == 1;
                    RadioButton_AllCompany.Visible = flag;
                    RadioButton_UsersAssets.Visible = flag;
                    RadioButton_AssetsEDI.Visible = flag;
                    if (!flag & (RadioButton_AllCompany.Checked | RadioButton_UsersAssets.Checked | RadioButton_AssetsEDI.Checked))
                    {
                        RadioButton_Nothing.Checked = true;
                    }
                }
                else
                {
                    TableUserExist = false;
                    Interaction.MsgBox("La table des sociétés est introuvable.", MsgBoxStyle.Exclamation);
                }
            }
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        private void ListUsers_DoubleClick(object sender, EventArgs e)
        {
            if ((interval_license > 0) | (ReturnCodeLicense == 3))
            {
                if (ReturnCodeLicense != 3)
                {
                    ReturnCodeLicense = 3;
                    MyProject.Computer.Registry.SetValue("HKEY_CURRENT_USER\\Software\\MeyoSoft", "License", ReturnCodeLicense);
                }
                Interaction.MsgBox("Your license expired. You must renew your license in order to run this software. Contact your vendor to renew your license.", MsgBoxStyle.Critical, MyProject.Application.Info.Title);
                base.Visible = false;
                Close();
            }
            if (!TableUserExist)
            {
                return;
            }
            checked
            {
                switch (Conversions.ToString(ListUsers.Items[ListUsers.SelectedIndex]))
                {
                    case "Consolidation":
                        mLaunchAppAndWait.LaunchAppAndWait(AppPath + "ACFRadio.exe", "/U=UFR02");
                        break;
                    case "Paramètres Environment Radio":
                        mLaunchAppAndWait.LaunchAppAndWait(AppPath + "AffConso\\ParamEnvironmentRadio.exe");
                        break;
                    case "Nomenclature":
                        mLaunchAppAndWait.LaunchAppAndWait(AppPath + "Nomencl.exe");
                        break;
                    case "Companies Access":
                        mLaunchAppAndWait.LaunchAppAndWait(AppPath + "Company_access\\CompaniesAccess.exe");
                        break;
                    case "Atelier Radio (Test)":
                        if ((interval > 0) | (ReturnCodeError == 2))
                        {
                            if (ReturnCodeError != 2)
                            {
                                ReturnCodeError = 2;
                                MyProject.Computer.Registry.SetValue("HKEY_CURRENT_USER\\Software\\MeyoSoft", "Acces", ReturnCodeError);
                            }
                            //_ = int.MaxValue * 2;
                            base.Visible = false;
                            Close();
                        }
                        mLaunchAppAndWait.LaunchAppAndWait(AppPath + "VERSION_TEST\\ATSAUVE.EXE", null, AppPath + "VERSION_TEST");
                        ProjectData.EndApp();
                        Close();
                        break;
                    default:
                        CompanySelected();
                        break;
                }
            }
        }

        private void Button_Param_Click(object sender, EventArgs e)
        {
            CheckedListBox_UsersEDI.Items.Clear();
            if (RadioButton_FolderEDI.Checked)
            {
                CheckFolderEDI();
            }
            else if (RadioButton_UsersAssets.Checked | RadioButton_AllCompany.Checked)
            {
                CheckUsers(RadioButton_AllCompany.Checked);
            }
            else if (RadioButton_AssetsEDI.Checked)
            {
                CheckUsers();
                CheckFolderEDI();
            }
            else
            {
                CheckDefault();
            }
            ListUsers.Visible = false;
            CheckedListBox_UsersEDI.Visible = true;
            Button_Launch.Visible = false;
            Button_Quitter.Visible = false;
            Button_Valid.Visible = true;
            Button_Cancel.Visible = true;
            LabelVeuillez.Text = "Veuillez selectionner des emails pour les retour EDI (MAX: " + (short)29 + "):";
        }

        private void Button_Cancel_Click(object sender, EventArgs e)
        {
            ListUsers.Visible = true;
            CheckedListBox_UsersEDI.Visible = false;
            Button_Launch.Visible = true;
            Button_Quitter.Visible = true;
            Button_Valid.Visible = false;
            Button_Cancel.Visible = false;
            LabelVeuillez.Text = "Veuillez choisir un environement:";
        }

        private void AddUnivers(string Univ)
        {
            if (File.Exists(AppPath + Univ))
            {
                ListUnivers.Add(AppPath + Univ);
            }
        }

        private void Button_Valid_Click(object sender, EventArgs e)
        {
            int num = 1;
            checked
            {
                IEnumerator enumerator = default(IEnumerator);
                try
                {
                    enumerator = CheckedListBox_UsersEDI.CheckedItems.GetEnumerator();
                    while (enumerator.MoveNext())
                    {
                        string text = Conversions.ToString(enumerator.Current);
                        mFileIni.Select_WriteIniString("Definition_du_site", "EInterloc" + num.ToString().Trim(), text.ToString().Trim(), AppPath + "Ataaetud.ini");
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
                if (num < 29)
                {
                    for (int i = num; i <= 29; i++)
                    {
                        mFileIni.Select_WriteIniString("Definition_du_site", "EInterloc" + i.ToString().Trim(), null, AppPath + "Ataaetud.ini");
                    }
                }
                ListUsers.Visible = true;
                CheckedListBox_UsersEDI.Visible = false;
                Button_Launch.Visible = true;
                Button_Quitter.Visible = true;
                Button_Valid.Visible = false;
                Button_Cancel.Visible = false;
                LabelVeuillez.Text = "Veuillez choisir un environemet:";
            }
        }

        private void CheckedListBox_UsersEDI_ItemCheck(object sender, ItemCheckEventArgs e)
        {
            if (!CheckedListBox_UsersEDI.CheckedItems.Contains(RuntimeHelpers.GetObjectValue(CheckedListBox_UsersEDI.Items[e.Index])) && CheckedListBox_UsersEDI.CheckedItems.Count >= 29)
            {
                Interaction.MsgBox("Maximun: " + (short)29, MsgBoxStyle.Exclamation);
                e.NewValue = CheckState.Unchecked;
            }
        }

        private void CheckedListBox_UsersEDI_DoubleClick(object sender, EventArgs e)
        {
            checked
            {
                int num = CheckedListBox_UsersEDI.Items.Count - 1;
                for (int i = 0; i <= num; i++)
                {
                    CheckedListBox_UsersEDI.SetItemCheckState(i, CheckState.Checked);
                    if (i == 28)
                    {
                        break;
                    }
                }
            }
        }

        private void ListUsers_SelectedIndexChanged(object sender, EventArgs e)
        {
            Text = "Atelier Radio (" + ListUsers.Text + ")";
        }

        private void CheckFolderEDI()
        {
            if (ListUsers.SelectedIndex == -1)
            {
                Interaction.MsgBox("Aucune sociétés sélectionnées.", MsgBoxStyle.Exclamation);
                return;
            }
            FOLDER_EXTERNE = TableUtilisateur[ListUsers.SelectedIndex, 2];
            USER_EMAIL = TableUtilisateur[ListUsers.SelectedIndex, 5];
            if ((Operators.CompareString(FOLDER_EXTERNE, null, TextCompare: false) == 0) | !Directory.Exists(FOLDER_EXTERNE))
            {
                return;
            }
            try
            {
                string[] array = (from w in Directory.GetFiles(FOLDER_EXTERNE + "\\Reception", "*.txt", SearchOption.TopDirectoryOnly)
                                  select (w) into w
                                  orderby File.GetLastWriteTime(w) descending
                                  select w).ToArray();
                if (array.Length <= 0)
                {
                    return;
                }
                string[] array2 = array;
                foreach (string path in array2)
                {
                    try
                    {
                        string text = Array.Find(File.ReadAllLines(path, Encoding.Default), TiersPO);
                        if (Operators.CompareString(text, null, TextCompare: false) == 0)
                        {
                            continue;
                        }
                        string[] array3 = Strings.Split(Strings.Replace(text, "\"", ""), ";");
                        if (Strings.StrComp(USER_EMAIL, array3[10], CompareMethod.Text) == 0)
                        {
                            continue;
                        }
                        if (CheckedListBox_UsersEDI.Items.Count > 0)
                        {
                            if (!CheckedListBox_UsersEDI.Items.Contains(array3[10]))
                            {
                                CheckedListBox_UsersEDI.Items.Add(array3[10]);
                            }
                        }
                        else
                        {
                            CheckedListBox_UsersEDI.Items.Add(array3[10]);
                        }
                    }
                    catch (Exception ex)
                    {
                        ProjectData.SetProjectError(ex);
                        Exception ex2 = ex;
                        Interaction.MsgBox(ex2.Message);
                        ProjectData.ClearProjectError();
                    }
                }
            }
            catch (Exception ex3)
            {
                ProjectData.SetProjectError(ex3);
                Exception ex4 = ex3;
                Interaction.MsgBox(ex4.Message, MsgBoxStyle.Critical);
                ProjectData.ClearProjectError();
            }
        }

        private void CheckUsers(bool AllCompany = false)
        {
            string[] array = (from x in Directory.GetFiles(Path.GetDirectoryName(PATH_FILE_TABLE_USER), "TableUtilisateur_*.tb?", SearchOption.TopDirectoryOnly)
                              select (x) into x
                              where Strings.StrComp(x, PATH_FILE_TABLE_USER, CompareMethod.Text) != 0
                              select x).ToArray();
            if (array.Length <= 0)
            {
                return;
            }
            string[] array2 = array;
            checked
            {
                for (int i = 0; i < array2.Length; i++)
                {
                    string[] array3 = File.ReadAllLines(array2[i], Encoding.Default);
                    foreach (string text in array3)
                    {
                        if (Operators.CompareString(text.Trim(), null, TextCompare: false) == 0)
                        {
                            continue;
                        }
                        string[] array4 = Strings.Split(text, ";");
                        if (AllCompany)
                        {
                            if (Operators.CompareString(array4[0], "", TextCompare: false) == 0)
                            {
                                int num = Information.UBound(array4);
                                for (int k = 1; k <= num; k++)
                                {
                                    array4[k - 1] = array4[k];
                                }
                                array4 = (string[])Utils.CopyArray(array4, new string[Information.UBound(array4) - 1 + 1]);
                            }
                            else
                            {
                                array4 = (string[])Utils.CopyArray(array4, new string[Information.UBound(array4) + 1]);
                            }
                        }
                        else
                        {
                            array4 = (string[])Utils.CopyArray(array4, new string[Information.UBound(array4) + 1]);
                        }
                        if (Strings.StrComp(ListUsers.Text, array4[0], CompareMethod.Text) != 0 || Strings.StrComp(USER_EMAIL, array4[5], CompareMethod.Text) == 0)
                        {
                            continue;
                        }
                        if (CheckedListBox_UsersEDI.Items.Count > 0)
                        {
                            if (!CheckedListBox_UsersEDI.Items.Contains(array4[5]))
                            {
                                CheckedListBox_UsersEDI.Items.Add(array4[5]);
                            }
                        }
                        else
                        {
                            CheckedListBox_UsersEDI.Items.Add(array4[5]);
                        }
                        break;
                    }
                }
            }
        }

        private void CheckDefault()
        {
            int num = 1;
            do
            {
                string NomModule = "Definition_du_site";
                string MotCle = "EInterloc" + num.ToString().Trim();
                string FichierIni = AppPath + "Ataaetud.ini";
                string text = mFileIni.Select_GetIniString(ref NomModule, ref MotCle, ref FichierIni).Trim();
                if (Operators.CompareString(text, null, TextCompare: false) != 0)
                {
                    CheckedListBox_UsersEDI.Items.Add(text, isChecked: true);
                    num = checked(num + 1);
                    continue;
                }
                break;
            }
            while (num <= 29);
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

        public bool IsDirectoryWritable(string path)
        {
            return (new DirectoryInfo(path).Attributes & FileAttributes.ReadOnly) != FileAttributes.ReadOnly;
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        private void ChangeCompanyOfCtachat(string svPathNomencl, string svSociete, string svEmail)
        {
            try
            {
                while (!Directory.Exists(svPathNomencl))
                {
                    switch (Interaction.MsgBox("Le chemin %FOLDER% n'est pas trouvé.".Replace("%FOLDER%", "\"" + svPathNomencl + "\""), MsgBoxStyle.AbortRetryIgnore | MsgBoxStyle.Critical | MsgBoxStyle.DefaultButton2, MyProject.Application.Info.Title))
                    {
                        case MsgBoxResult.Ignore:
                            HttpSendMessage(33, COMPANY_NAME, "Folder Not Found", "Ignore folder");
                            return;
                        case MsgBoxResult.Abort:
                            HttpSendMessage(33, COMPANY_NAME, "Folder Not Found", "Abort");
                            ProjectData.EndApp();
                            break;
                    }
                    Thread.Sleep(500);
                    Application.DoEvents();
                }
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                Interaction.MsgBox(ex2.Message, MsgBoxStyle.Critical, "chemin non trouvé");
                HttpSendMessage(33, COMPANY_NAME, "Error", "Folder Not Found");
                ProjectData.ClearProjectError();
                return;
            }
            try
            {
                while (!IsDirectoryWritable(svPathNomencl))
                {
                    switch (Interaction.MsgBox("L'accès au chemin %FOLDER% est refusé.".Replace("%FOLDER%", "\"" + svPathNomencl + "\""), MsgBoxStyle.AbortRetryIgnore | MsgBoxStyle.Critical | MsgBoxStyle.DefaultButton2, MyProject.Application.Info.Title))
                    {
                        case MsgBoxResult.Ignore:
                            HttpSendMessage(33, COMPANY_NAME, "Access denied", "Ignore access");
                            return;
                        case MsgBoxResult.Abort:
                            HttpSendMessage(33, COMPANY_NAME, "Access denied", "Abort");
                            ProjectData.EndApp();
                            break;
                    }
                    Thread.Sleep(500);
                    Application.DoEvents();
                }
            }
            catch (Exception ex3)
            {
                ProjectData.SetProjectError(ex3);
                Exception ex4 = ex3;
                Interaction.MsgBox(ex4.Message, MsgBoxStyle.Critical, "Accès refusé");
                HttpSendMessage(33, COMPANY_NAME, "Error", "Access denied");
                ProjectData.ClearProjectError();
                return;
            }
            string text = svPathNomencl + "Ctachat.arb";
            try
            {
                while (FileIsLocked(text))
                {
                    switch (Interaction.MsgBox("L'accès à la nomenclature %CTACHAT% est refusé.".Replace("%CTACHAT%", "\"" + svPathNomencl + "\""), MsgBoxStyle.AbortRetryIgnore | MsgBoxStyle.Critical | MsgBoxStyle.DefaultButton2, MyProject.Application.Info.Title))
                    {
                        case MsgBoxResult.Ignore:
                            HttpSendMessage(33, COMPANY_NAME, "Blocked file", "Ignore file");
                            return;
                        case MsgBoxResult.Abort:
                            HttpSendMessage(33, COMPANY_NAME, "Blocked file", "Abort");
                            ProjectData.EndApp();
                            break;
                    }
                    Thread.Sleep(500);
                    Application.DoEvents();
                }
            }
            catch (Exception ex5)
            {
                ProjectData.SetProjectError(ex5);
                Exception ex6 = ex5;
                Interaction.MsgBox(ex6.Message, MsgBoxStyle.Critical, "Fichier bloqué");
                HttpSendMessage(33, COMPANY_NAME, "Error", "Blocked file");
                ProjectData.ClearProjectError();
                return;
            }
            string text2 = svPathNomencl + "Backup\\";
            string text3 = svPathNomencl + "Ctachat.lck";
            string text4 = null;
            int num = 0;
            bool flag = false;
            string text5 = MyProject.User.Name;
            int num2 = Strings.InStr(text5, "\\");
            checked
            {
                if (num2 > 0)
                {
                    text5 = Strings.Mid(text5, num2 + 1, Strings.Len(text5) - num2);
                }
                text5 = MyProject.Computer.Name + "\\" + text5;
                text4 = "0x00";
                try
                {
                    while (File.Exists(text3))
                    {
                        if (FileSystem.FileLen(text3) == 0L)
                        {
                            File.Delete(text3);
                            HttpSendMessage(33, COMPANY_NAME, "Error", "FileLen");
                            break;
                        }
                        text4 = "0x01";
                        string text6 = File.ReadAllText(text3, Encoding.Default);
                        if (Strings.StrComp(text5, text6, CompareMethod.Text) == 0)
                        {
                            text4 = "0x02";
                            File.Delete(text3);
                        }
                        Thread.Sleep(500);
                        num++;
                        Application.DoEvents();
                        if (num > 20)
                        {
                            if (Interaction.MsgBox("Impossible d'initialiser l'EDI", MsgBoxStyle.RetryCancel | MsgBoxStyle.Exclamation, text6) == MsgBoxResult.Retry)
                            {
                                num = 0;
                                continue;
                            }
                            HttpSendMessage(33, COMPANY_NAME, "Error", text4);
                            ProjectData.EndApp();
                        }
                    }
                    text4 = "0x03";
                    File.WriteAllText(text3, text5);
                    ArrayList arrayList = new ArrayList();
                    text4 = "0x04";
                    StreamReader streamReader = MyProject.Computer.FileSystem.OpenTextFileReader(text, Encoding.Default);
                    arrayList.Clear();
                    string text7;
                    do
                    {
                        text7 = streamReader.ReadLine();
                        arrayList.Add(text7);
                    }
                    while (text7 != null);
                    streamReader.Close();
                    text4 = "0x05";
                    while (Operators.CompareString(Conversions.ToString(arrayList[arrayList.Count - 1]), null, TextCompare: false) == 0)
                    {
                        arrayList.RemoveAt(arrayList.Count - 1);
                    }
                    text4 = "0x06";
                    int num3 = arrayList.IndexOf(svEmail);
                    text4 = "0x07";
                    if (num3 == -1)
                    {
                        int num4 = arrayList.IndexOf(svSociete);
                        text4 = "0x08";
                        if (num4 > 0)
                        {
                            string svNom = null;
                            string svPrenom = null;
                            ExtractNameOfMail(svEmail, ref svNom, ref svPrenom);
                            text4 = "0x0A";
                            int num5 = arrayList.Count - 1;
                            for (int i = num4; i <= num5; i++)
                            {
                                int result = -1;
                                if (unchecked((Conversions.ToString(arrayList[i]).Contains("#") & int.TryParse(Strings.Mid(Conversions.ToString(arrayList[i]), 2, checked(Strings.Len(RuntimeHelpers.GetObjectValue(arrayList[i])) - 1)), out result)) && result >= 0))
                                {
                                    result++;
                                    arrayList[i] = "#" + Conversions.ToString(result).Trim();
                                    arrayList.Insert(i + 1, "#0");
                                    arrayList.Insert(i + 1, svEmail);
                                    arrayList.Insert(i + 1, svPrenom);
                                    arrayList.Insert(i + 1, svNom);
                                    break;
                                }
                            }
                            text4 = "0x0B";
                            flag = true;
                        }
                    }
                    else
                    {
                        text4 = "0x0C";
                        if (arrayList.IndexOf(svEmail, num3 + 1) > num3)
                        {
                            text4 = "0x0D";
                            while (arrayList.IndexOf(svEmail, num3 + 1) > num3)
                            {
                                text4 = "0x0E";
                                int num6 = arrayList.IndexOf(svEmail, num3 + 1);
                                text4 = "0x0F";
                                int num7 = num3;
                                for (int j = num6; j >= num7; j += -1)
                                {
                                    int result2 = -1;
                                    if (unchecked((Conversions.ToString(arrayList[j]).Contains("#") & int.TryParse(Strings.Mid(Conversions.ToString(arrayList[j]), 2, checked(Strings.Len(RuntimeHelpers.GetObjectValue(arrayList[j])) - 1)), out result2)) && result2 > 0))
                                    {
                                        result2--;
                                        arrayList[j] = "#" + Conversions.ToString(result2).Trim();
                                        break;
                                    }
                                }
                                text4 = "0x10";
                                arrayList.RemoveAt(num6 + 1);
                                arrayList.RemoveAt(num6);
                                arrayList.RemoveAt(num6 - 1);
                                arrayList.RemoveAt(num6 - 2);
                                text4 = "0x11";
                            }
                            flag = true;
                        }
                        text4 = "0x12";
                        if (Operators.CompareString(GetSocieteToEmail(arrayList, num3), svSociete, TextCompare: false) != 0)
                        {
                            text4 = "0x13";
                            int num8 = arrayList.IndexOf(svSociete);
                            text4 = "0x14";
                            if (num8 > 0)
                            {
                                text4 = "0x15";
                                for (int k = num3; k >= 0; k += -1)
                                {
                                    int result3 = -1;
                                    if (unchecked((Conversions.ToString(arrayList[k]).Contains("#") & int.TryParse(Strings.Mid(Conversions.ToString(arrayList[k]), 2, checked(Strings.Len(RuntimeHelpers.GetObjectValue(arrayList[k])) - 1)), out result3)) && result3 > 0))
                                    {
                                        result3--;
                                        arrayList[k] = "#" + Conversions.ToString(result3).Trim();
                                        break;
                                    }
                                }
                                text4 = "0x16";
                                string value = Conversions.ToString(arrayList[num3 - 2]);
                                string value2 = Conversions.ToString(arrayList[num3 - 1]);
                                arrayList.RemoveAt(num3 + 1);
                                arrayList.RemoveAt(num3);
                                arrayList.RemoveAt(num3 - 1);
                                arrayList.RemoveAt(num3 - 2);
                                text4 = "0x17";
                                int num9 = arrayList.Count - 1;
                                for (int l = num8; l <= num9; l++)
                                {
                                    int result4 = -1;
                                    if (unchecked((Conversions.ToString(arrayList[l]).Contains("#") & int.TryParse(Strings.Mid(Conversions.ToString(arrayList[l]), 2, checked(Strings.Len(RuntimeHelpers.GetObjectValue(arrayList[l])) - 1)), out result4)) && result4 >= 0))
                                    {
                                        result4++;
                                        arrayList[l] = "#" + Conversions.ToString(result4).Trim();
                                        arrayList.Insert(l + 1, "#0");
                                        arrayList.Insert(l + 1, svEmail);
                                        arrayList.Insert(l + 1, value2);
                                        arrayList.Insert(l + 1, value);
                                        break;
                                    }
                                }
                                text4 = "0x18";
                                flag = true;
                            }
                        }
                    }
                    text4 = "0x19";
                    if (flag)
                    {
                        text4 = "0x1A";
                        text4 = "0x1B";
                        if (File.Exists(text + ".tmp"))
                        {
                            text4 = "0x1C";
                            File.Delete(text + ".tmp");
                        }
                        text4 = "0x1D";
                        StreamWriter streamWriter = MyProject.Computer.FileSystem.OpenTextFileWriter(text + ".tmp", false, Encoding.Default);
                        text4 = "0x1E";
                        IEnumerator enumerator = default(IEnumerator);
                        try
                        {
                            enumerator = arrayList.GetEnumerator();
                            while (enumerator.MoveNext())
                            {
                                string value3 = Conversions.ToString(enumerator.Current);
                                streamWriter.WriteLine(value3);
                            }
                        }
                        finally
                        {
                            if (enumerator is IDisposable)
                            {
                                (enumerator as IDisposable).Dispose();
                            }
                        }
                        text4 = "0x1F";
                        streamWriter.Close();
                        text4 = "0x20";
                        if (!Directory.Exists(text2))
                        {
                            text4 = "0x21";
                            Directory.CreateDirectory(text2);
                        }
                        text4 = "0x22";
                        string text8 = text2 + "Ctachat_" + text5.Replace(".", "_").Replace("\\", "_") + "_" + DateAndTime.Now.ToString().Replace("/", null).Replace(" ", null)
                            .Replace(":", null) + ".arb";
                        string text9 = text + ".tmp";
                        try
                        {
                            File.Replace(text9, text, text8, ignoreMetadataErrors: true);
                        }
                        catch (Exception ex7)
                        {
                            ProjectData.SetProjectError(ex7);
                            Exception ex8 = ex7;
                            Interaction.MsgBox(ex8.Message + "(" + text4 + ")\r\n" + text5 + "\r\n" + text9 + "\r\n" + text + "\r\n" + text8, MsgBoxStyle.Critical);
                            HttpSendMessage(33, COMPANY_NAME, "Error", text4);
                            ProjectData.EndApp();
                            ProjectData.ClearProjectError();
                        }
                        PurgeBackupFile(text2);
                        string[] array = File.ReadAllLines(text, Encoding.Default);
                        text4 = "0x23";
                        EMAIL_VERIF = svEmail;
                        int num10 = Array.FindIndex(array, FindEmail);
                        text4 = "0x24";
                        for (int m = num10; m >= 0; m += -1)
                        {
                            if (array[m].Contains("PO"))
                            {
                                if (Operators.CompareString(svSociete, array[m + 3], TextCompare: false) == 0)
                                {
                                    break;
                                }
                                Interaction.MsgBox("Erreur d'ecriture dans le fichier %FILE%\r\n\r\nL'application va se fermer.".Replace("%FILE%", "\"" + text + "\""), MsgBoxStyle.Critical, MyProject.Application.Info.Title);
                                HttpSendMessage(33, COMPANY_NAME, "Error", text4);
                                ProjectData.EndApp();
                            }
                        }
                    }
                    text4 = "0x25";
                    File.Delete(text3);
                }
                catch (Exception ex9)
                {
                    ProjectData.SetProjectError(ex9);
                    Exception ex10 = ex9;
                    HttpSendMessage(33, COMPANY_NAME, "Error", text4);
                    if (File.Exists(text3))
                    {
                        File.Delete(text3);
                    }
                    if (File.Exists(svPathNomencl + "Ctachat.bak"))
                    {
                        File.Delete(svPathNomencl + "Ctachat.bak");
                    }
                    Interaction.MsgBox(ex10.Message + "(" + text4 + ")", MsgBoxStyle.Critical);
                    ProjectData.EndApp();
                    ProjectData.ClearProjectError();
                }
            }
        }

        private bool FindEmail(string p)
        {
            if (Operators.CompareString(p, EMAIL_VERIF, TextCompare: false) == 0)
            {
                return true;
            }
            return false;
        }

        private void ExtractNameOfMail(string svEmail, ref string svNom, ref string svPrenom)
        {
            int num = Strings.InStr(svEmail, "@");
            checked
            {
                if (num > 0)
                {
                    svNom = Strings.Mid(svEmail, 1, num - 1);
                }
                int num2 = Strings.InStr(svNom, ".");
                if (num2 > 0)
                {
                    svPrenom = Strings.StrConv(Strings.Mid(svNom, 1, num2 - 1), VbStrConv.ProperCase);
                    svNom = Strings.StrConv(Strings.Mid(svNom, num2 + 1, Strings.Len(svNom) - num2), VbStrConv.ProperCase);
                }
                else
                {
                    svPrenom = svNom;
                }
                if (Operators.CompareString(svNom, null, TextCompare: false) == 0)
                {
                    svNom = "Nom";
                }
                if (Operators.CompareString(svPrenom, null, TextCompare: false) == 0)
                {
                    svPrenom = "Prenom";
                }
            }
        }

        private bool FileIsLocked(string fileFullPathName)
        {
            bool result = false;
            FileStream fileStream = null;
            try
            {
                fileStream = new FileStream(fileFullPathName, FileMode.Open, FileAccess.ReadWrite, FileShare.None);
                return result;
            }
            catch (Exception projectError)
            {
                ProjectData.SetProjectError(projectError);
                result = true;
                ProjectData.ClearProjectError();
                return result;
            }
            finally
            {
                fileStream?.Close();
            }
        }

        private bool IsFileLocked(string FileToCheck)
        {
            try
            {
                MyProject.Computer.FileSystem.GetFileInfo(FileToCheck).Open(FileMode.Open, FileAccess.Write, FileShare.Delete);
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                bool result = true;
                ProjectData.ClearProjectError();
                return result;
            }
            return false;
        }

        private void PurgeBackupFile(string svPathPurge)
        {
            int num = 60;
            checked
            {
                try
                {
                    string[] files = Directory.GetFiles(svPathPurge, "Ctachat*.arb", SearchOption.TopDirectoryOnly);
                    files = (from w in files
                             select (w) into w
                             orderby File.GetLastWriteTime(w)
                             select w).ToArray();
                    if (files.Length > num)
                    {
                        int num2 = files.Length - num - 1;
                        for (int i = 0; i <= num2; i++)
                        {
                            File.Delete(files[i]);
                        }
                    }
                }
                catch (Exception ex)
                {
                    ProjectData.SetProjectError(ex);
                    Exception ex2 = ex;
                    Interaction.MsgBox(ex2.Message, MsgBoxStyle.Critical, "Purger des fichiers de sauvegarde");
                    ProjectData.ClearProjectError();
                }
            }
        }

        private string GetSocieteToEmail(ArrayList ListToFind, int IndexOfEmail)
        {
            checked
            {
                for (int i = IndexOfEmail; i >= 0; i += -1)
                {
                    if (Operators.CompareString(Conversions.ToString(ListToFind[i]), "PO", TextCompare: false) == 0)
                    {
                        return Conversions.ToString(ListToFind[i + 3]);
                    }
                }
                return null;
            }
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        private void FindAllEmail(string svPathNomencl)
        {
            try
            {
                StreamReader streamReader = MyProject.Computer.FileSystem.OpenTextFileReader(svPathNomencl + "Ctachat.arb", Encoding.Default);
                StreamWriter streamWriter = MyProject.Computer.FileSystem.OpenTextFileWriter(svPathNomencl + "Ctachat_Email.txt", false, Encoding.Default);
                string text;
                do
                {
                    text = streamReader.ReadLine();
                    try
                    {
                        if (text.Contains("@"))
                        {
                            streamWriter.WriteLine(text);
                        }
                    }
                    catch (Exception ex)
                    {
                        ProjectData.SetProjectError(ex);
                        Exception ex2 = ex;
                        ProjectData.ClearProjectError();
                    }
                }
                while (text != null);
                streamReader.Close();
                streamWriter.Close();
            }
            catch (Exception ex3)
            {
                ProjectData.SetProjectError(ex3);
                Exception ex4 = ex3;
                Interaction.MsgBox(ex4.Message, MsgBoxStyle.Critical);
                ProjectData.EndApp();
                ProjectData.ClearProjectError();
            }
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        private void FindAllSociete(string svPathNomencl)
        {
            try
            {
                StreamReader streamReader = MyProject.Computer.FileSystem.OpenTextFileReader(svPathNomencl + "Ctachat.arb", Encoding.Default);
                StreamWriter streamWriter = MyProject.Computer.FileSystem.OpenTextFileWriter(svPathNomencl + "Ctachat_Societe.txt", false, Encoding.Default);
                string text;
                do
                {
                    text = streamReader.ReadLine();
                    try
                    {
                        if (Operators.CompareString(text, "PO", TextCompare: false) == 0)
                        {
                            text = streamReader.ReadLine();
                            text = streamReader.ReadLine();
                            text = streamReader.ReadLine();
                            streamWriter.WriteLine(text);
                        }
                    }
                    catch (Exception ex)
                    {
                        ProjectData.SetProjectError(ex);
                        Exception ex2 = ex;
                        ProjectData.ClearProjectError();
                    }
                }
                while (text != null);
                streamReader.Close();
                streamWriter.Close();
            }
            catch (Exception ex3)
            {
                ProjectData.SetProjectError(ex3);
                Exception ex4 = ex3;
                Interaction.MsgBox(ex4.Message, MsgBoxStyle.Critical);
                ProjectData.EndApp();
                ProjectData.ClearProjectError();
            }
        }

        public string HttpSendMessage(int code, string svWebCompanyName = null, string maj = null, string info = null)
        {
            return null;
        }

        private string Truncate(string svString, int nvLen)
        {
            if (Strings.Len(svString) > nvLen)
            {
                return Strings.Mid(svString, 1, nvLen);
            }
            return svString;
        }

        private string Deplacement(string[] Societe, string szFolder)
        {
            if ((Operators.CompareString(USER_TABLE, null, TextCompare: false) == 0) | (Operators.CompareString(N_CLIENT, null, TextCompare: false) == 0))
            {
                return null;
            }
            if (Directory.Exists(szFolder))
            {
                string[] array;
                try
                {
                    array = (from w in Directory.GetFiles(szFolder, "*.TXT", SearchOption.TopDirectoryOnly)
                             select (w) into w
                             orderby File.GetLastWriteTime(w) descending
                             select w).ToArray();
                }
                catch (Exception ex)
                {
                    ProjectData.SetProjectError(ex);
                    Exception ex2 = ex;
                    Interaction.MsgBox(ex2.Message, MsgBoxStyle.Critical);
                    string result = null;
                    ProjectData.ClearProjectError();
                    return result;
                }
                if (array.Length > 0)
                {
                    string[] array2 = array;
                    foreach (string text in array2)
                    {
                        try
                        {
                            string[] array3 = File.ReadAllLines(text, Encoding.Default);
                            string text2 = Array.Find(array3, TiersPO);
                            if (Operators.CompareString(text2, null, TextCompare: false) == 0)
                            {
                                continue;
                            }
                            string[] array4 = Strings.Split(Strings.Replace(text2, "\"", ""), ";");
                            if (Operators.CompareString(array4[4], "KRMED", TextCompare: false) == 0)
                            {
                                array4[4] = "KR MEDIA";
                            }
                            string siret = GetSiret(N_CLIENT, array4[4]);
                            if (!(!Societe.Contains(array4[4]) & (Operators.CompareString(siret, null, TextCompare: false) != 0)))
                            {
                                continue;
                            }
                            string[] array5 = File.ReadAllLines(USER_TABLE, Encoding.Default);
                            foreach (string text3 in array5)
                            {
                                if (Operators.CompareString(text3.Trim(), null, TextCompare: false) == 0)
                                {
                                    continue;
                                }
                                string[] array6 = Strings.Split(text3, ";");
                                if (Strings.StrComp(array4[4], array6[0], CompareMethod.Text) != 0)
                                {
                                    continue;
                                }
                                string text4 = array6[2];
                                int num = Array.IndexOf(array3, text2);
                                array3[num] = text2.Replace(array4[2], siret);
                                string text5 = mFileFunction.ParsePath(text, 3);
                                if (!(!File.Exists(text + ".MOV") & !File.Exists(text4 + "\\RECEPTION\\" + text5)))
                                {
                                    break;
                                }
                                try
                                {
                                    if (File.Exists(text4 + "\\RECEPTION\\" + text5))
                                    {
                                        File.Delete(text4 + "\\RECEPTION\\" + text5);
                                    }
                                    File.Move(text, text + ".MOV");
                                    File.WriteAllLines(text4 + "\\RECEPTION\\" + text5, array3, Encoding.Default);
                                }
                                catch (Exception ex3)
                                {
                                    ProjectData.SetProjectError(ex3);
                                    Exception ex4 = ex3;
                                    HttpSendMessage(30, Societe[0], array4[4], "Error!!!");
                                    ProjectData.ClearProjectError();
                                    goto IL_0388;
                                }
                                string text6 = Array.Find(array3, Campagne);
                                string text7 = Array.Find(array3, TiersZDS);
                                if ((Operators.CompareString(text6, null, TextCompare: false) != 0) & (Operators.CompareString(text7, null, TextCompare: false) != 0))
                                {
                                    string[] array7 = Strings.Split(Strings.Replace(text6, "\"", ""), ";");
                                    string[] array8 = Strings.Split(Strings.Replace(text7, "\"", ""), ";");
                                    HttpSendMessage(30, Societe[0], array4[4] + "/" + array8[4], array7[1]);
                                }
                                else
                                {
                                    HttpSendMessage(30, Societe[0], array4[4]);
                                }
                                break;
                            }
                        }
                        catch (Exception ex5)
                        {
                            ProjectData.SetProjectError(ex5);
                            Exception ex6 = ex5;
                            ProjectData.ClearProjectError();
                        }
                        IL_0388:;
                    }
                }
            }
            return null;
        }

        private string GetSiret(string svPathNomencl, string svSociete)
        {
            ArrayList arrayList = new ArrayList();
            string file = svPathNomencl + "Ctachat.arb";
            StreamReader streamReader = MyProject.Computer.FileSystem.OpenTextFileReader(file, Encoding.Default);
            arrayList.Clear();
            string text;
            do
            {
                text = streamReader.ReadLine();
                arrayList.Add(text);
            }
            while (text != null);
            streamReader.Close();
            int num = arrayList.IndexOf(svSociete);
            if (num >= 0)
            {
                return Strings.Split(Conversions.ToString(arrayList[checked(num - 2)]), "&")[0];
            }
            return null;
        }

        private int FileGrep(string szFileName, string szSearchStr, ref string svReturnLine, ref int nvLineNumber, int nFlag)
        {
            string[] source = File.ReadAllLines(szFileName, Encoding.Default);
            checked
            {
                if (nFlag == 1)
                {
                    nvLineNumber++;
                }
                if (nFlag == 0)
                {
                    nvLineNumber = 0;
                }
                foreach (string item in source.Skip(nvLineNumber))
                {
                    if (item.Contains(szSearchStr))
                    {
                        svReturnLine = item;
                        return 0;
                    }
                    nvLineNumber++;
                }
                return -1;
            }
        }

        private int FileInsertLine(string szFileName, string szInsertLine, int nLineNumber, int nInsertFlag)
        {
            string[] c = File.ReadAllLines(szFileName, Encoding.Default);
            ArrayList arrayList = new ArrayList();
            arrayList.AddRange(c);
            switch (nInsertFlag)
            {
                case 0:
                    arrayList.RemoveAt(nLineNumber);
                    arrayList.Insert(nLineNumber, szInsertLine);
                    break;
                case -1:
                    arrayList.Insert(nLineNumber, szInsertLine);
                    break;
                case 1:
                    arrayList.Insert(checked(nLineNumber + 1), szInsertLine);
                    break;
                default:
                    return -1;
            }
            try
            {
                File.Move(szFileName, szFileName + ".BAK");
                StreamWriter streamWriter = new StreamWriter(szFileName);
                IEnumerator enumerator = default(IEnumerator);
                try
                {
                    enumerator = arrayList.GetEnumerator();
                    while (enumerator.MoveNext())
                    {
                        string value = Conversions.ToString(enumerator.Current);
                        streamWriter.WriteLine(value);
                    }
                }
                finally
                {
                    if (enumerator is IDisposable)
                    {
                        (enumerator as IDisposable).Dispose();
                    }
                }
                streamWriter.Close();
            }
            catch (Exception ex)
            {
                ProjectData.SetProjectError(ex);
                Exception ex2 = ex;
                int result = -1;
                ProjectData.ClearProjectError();
                return result;
            }
            return 0;
        }

        private string Url_Encode(string svUrl)
        {
            if (Operators.CompareString(svUrl, null, TextCompare: false) == 0)
            {
                return null;
            }
            svUrl = svUrl.Replace(" ", "%20");
            svUrl = svUrl.Replace("!", "%21");
            svUrl = svUrl.Replace("$", "%24");
            svUrl = svUrl.Replace("&", "%26");
            svUrl = svUrl.Replace("'", "%27");
            svUrl = svUrl.Replace("(", "%28");
            svUrl = svUrl.Replace(")", "%29");
            svUrl = svUrl.Replace("@", "%40");
            svUrl = svUrl.Replace("À", "%C0");
            svUrl = svUrl.Replace("Á", "%C1");
            svUrl = svUrl.Replace("Â", "%C2");
            svUrl = svUrl.Replace("Ã", "%C3");
            svUrl = svUrl.Replace("Ä", "%C4");
            svUrl = svUrl.Replace("Å", "%C5");
            svUrl = svUrl.Replace("Æ", "%C6");
            svUrl = svUrl.Replace("Ç", "%C7");
            svUrl = svUrl.Replace("È", "%C8");
            svUrl = svUrl.Replace("É", "%C9");
            svUrl = svUrl.Replace("Ê", "%CA");
            svUrl = svUrl.Replace("Ë", "%CB");
            svUrl = svUrl.Replace("Ì", "%CC");
            svUrl = svUrl.Replace("Í", "%CD");
            svUrl = svUrl.Replace("Î", "%CE");
            svUrl = svUrl.Replace("Ï", "%CF");
            svUrl = svUrl.Replace("Ð", "%D0");
            svUrl = svUrl.Replace("Ñ", "%D1");
            svUrl = svUrl.Replace("Ò", "%D2");
            svUrl = svUrl.Replace("Ó", "%D3");
            svUrl = svUrl.Replace("Ô", "%D4");
            svUrl = svUrl.Replace("Õ", "%D5");
            svUrl = svUrl.Replace("Ö", "%D6");
            svUrl = svUrl.Replace("×", "%D7");
            svUrl = svUrl.Replace("Ù", "%D9");
            svUrl = svUrl.Replace("Ú", "%DA");
            svUrl = svUrl.Replace("Û", "%DB");
            svUrl = svUrl.Replace("Ü", "%DC");
            svUrl = svUrl.Replace("Ý", "%DD");
            svUrl = svUrl.Replace("à", "%E0");
            svUrl = svUrl.Replace("á", "%E1");
            svUrl = svUrl.Replace("â", "%E2");
            svUrl = svUrl.Replace("ã", "%E3");
            svUrl = svUrl.Replace("ä", "%E4");
            svUrl = svUrl.Replace("å", "%E5");
            svUrl = svUrl.Replace("æ", "%E6");
            svUrl = svUrl.Replace("ç", "%E7");
            svUrl = svUrl.Replace("è", "%E8");
            svUrl = svUrl.Replace("é", "%E9");
            svUrl = svUrl.Replace("ê", "%EA");
            svUrl = svUrl.Replace("ë", "%EB");
            svUrl = svUrl.Replace("ì", "%EC");
            svUrl = svUrl.Replace("í", "%ED");
            svUrl = svUrl.Replace("î", "%EE");
            svUrl = svUrl.Replace("ï", "%EF");
            svUrl = svUrl.Replace("ð", "%F0");
            svUrl = svUrl.Replace("ñ", "%F1");
            svUrl = svUrl.Replace("ò", "%F2");
            svUrl = svUrl.Replace("ó", "%F3");
            svUrl = svUrl.Replace("ô", "%F4");
            svUrl = svUrl.Replace("õ", "%F5");
            svUrl = svUrl.Replace("ö", "%F6");
            svUrl = svUrl.Replace("ù", "%F9");
            svUrl = svUrl.Replace("ú", "%FA");
            svUrl = svUrl.Replace("û", "%FB");
            svUrl = svUrl.Replace("ü", "%FC");
            svUrl = svUrl.Replace("ý", "%FD");
            svUrl = svUrl.Replace("þ", "%FE");
            svUrl = svUrl.Replace("ÿ", "%FF");
            return svUrl;
        }

        [MethodImpl(MethodImplOptions.NoInlining | MethodImplOptions.NoOptimization)]
        private void Button_Launch_Click(object sender, EventArgs e)
        {
            if ((interval_license > 0) | (ReturnCodeLicense == 3))
            {
                if (ReturnCodeLicense != 3)
                {
                    ReturnCodeLicense = 3;
                    MyProject.Computer.Registry.SetValue("HKEY_CURRENT_USER\\Software\\MeyoSoft", "License", ReturnCodeLicense);
                }
                Interaction.MsgBox("Your license expired. You must renew your license in order to run this software. Contact your vendor to renew your license.", MsgBoxStyle.Critical, MyProject.Application.Info.Title);
                base.Visible = false;
                Close();
            }
            if (!TableUserExist)
            {
                return;
            }
            checked
            {
                if (ListUsers.SelectedIndex >= 0)
                {
                    switch (Conversions.ToString(ListUsers.Items[ListUsers.SelectedIndex]))
                    {
                        case "Consolidation":
                            mLaunchAppAndWait.LaunchAppAndWait(AppPath + "ACFRadio.exe", "/U=UFR02");
                            break;
                        case "Paramètres Environment Radio":
                            mLaunchAppAndWait.LaunchAppAndWait(AppPath + "AffConso\\ParamEnvironmentRadio.exe");
                            break;
                        case "Nomenclature":
                            mLaunchAppAndWait.LaunchAppAndWait(AppPath + "Nomencl.exe");
                            break;
                        case "Companies Access":
                            mLaunchAppAndWait.LaunchAppAndWait(AppPath + "Company_access\\CompaniesAccess.exe");
                            break;
                        case "Atelier Radio (Test)":
                            if ((interval > 0) | (ReturnCodeError == 2))
                            {
                                if (ReturnCodeError != 2)
                                {
                                    ReturnCodeError = 2;
                                    MyProject.Computer.Registry.SetValue("HKEY_CURRENT_USER\\Software\\MeyoSoft", "Acces", ReturnCodeError);
                                }
                                //_ = int.MaxValue * 2;
                                base.Visible = false;
                                Close();
                            }
                            mLaunchAppAndWait.LaunchAppAndWait(AppPath + "VERSION_TEST\\ATSAUVE.EXE", null, AppPath + "VERSION_TEST");
                            ProjectData.EndApp();
                            Close();
                            break;
                        default:
                            CompanySelected();
                            break;
                    }
                }
                else if (ListUsers.Items.Count > 0)
                {
                    Interaction.MsgBox("Aucune sociétés sélectionnées.", MsgBoxStyle.Exclamation);
                }
                else
                {
                    mLaunchAppAndWait.LaunchAppAndWait(AppPath + "ATSAUVE.EXE");
                    Close();
                }
            }
        }

        private void SaveSas(string szfilename, string szCompany)
        {
            if (!(File.Exists(szfilename) & (Operators.CompareString(szCompany, "", TextCompare: false) != 0)))
            {
                return;
            }
            List<string> list = File.ReadAllLines(szfilename).ToList();
            bool flag = false;
            int num = 0;
            num = list.FindIndex((string value) => value.Contains("Objets SC="));
            if (num > -1)
            {
                list[num] = list[num].Replace("Objets SC=", "Objets SC " + szCompany + "=");
                flag = true;
                num = list.FindIndex((string value) => value.Contains("[Sas Campagne]"));
                if (num > -1)
                {
                    list[num] = list[num].Replace("[Sas Campagne]", "[Sas Campagne " + szCompany + "]");
                }
            }
            num = list.FindIndex((string value) => value.Contains("Objets SE="));
            if (num > -1)
            {
                list[num] = list[num].Replace("Objets SE=", "Objets SE " + szCompany + "=");
                flag = true;
                num = list.FindIndex((string value) => value.Contains("[Sas Creuset]"));
                if (num > -1)
                {
                    list[num] = list[num].Replace("[Sas Creuset]", "[Sas Creuset " + szCompany + "]");
                }
            }
            if (flag)
            {
                File.WriteAllLines(szfilename, list);
            }
        }

        private void RestoreSas(string szfilename, string szCompany)
        {
            if (!(File.Exists(szfilename) & (Operators.CompareString(szCompany, "", TextCompare: false) != 0)))
            {
                return;
            }
            List<string> list = File.ReadAllLines(szfilename).ToList();
            bool flag = false;
            int num = 0;
            num = list.FindIndex((string value) => value.Contains("Objets SE " + szCompany + "="));
            if (num > -1)
            {
                list[num] = list[num].Replace("Objets SE " + szCompany + "=", "Objets SE=");
                flag = true;
                num = list.FindIndex((string value) => value.Contains("[Sas Creuset " + szCompany + "]"));
                if (num > -1)
                {
                    list[num] = list[num].Replace("[Sas Creuset " + szCompany + "]", "[Sas Creuset]");
                }
            }
            num = list.FindIndex((string value) => value.Contains("Objets SC " + szCompany + "="));
            if (num > -1)
            {
                list[num] = list[num].Replace("Objets SC " + szCompany + "=", "Objets SC=");
                flag = true;
                num = list.FindIndex((string value) => value.Contains("[Sas Campagne " + szCompany + "]"));
                if (num > -1)
                {
                    list[num] = list[num].Replace("[Sas Campagne " + szCompany + "]", "[Sas Campagne]");
                }
            }
            if (flag)
            {
                File.WriteAllLines(szfilename, list);
            }
        }
    }

}
