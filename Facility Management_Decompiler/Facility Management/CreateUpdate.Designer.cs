
using Facility_Management.Properties;
using System;
using System.ComponentModel;
using System.Drawing;
using System.Runtime.CompilerServices;
using System.Windows.Forms;
using Facility_Management.Properties;

namespace Facility_Management
{
    partial class CreateUpdate
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

        #region Windows Form Designer generated code

        [System.Diagnostics.DebuggerStepThrough]
        private void InitializeComponent()
        {
            //System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Facility_Management.CreateUpdate));
            Label2 = new System.Windows.Forms.Label();
            Label1 = new System.Windows.Forms.Label();
            ComboBoxPatchName = new System.Windows.Forms.ComboBox();
            TextPatchInformation = new System.Windows.Forms.TextBox();
            OpenFileDialogVersion = new System.Windows.Forms.OpenFileDialog();
            ButtonGetTarifs = new System.Windows.Forms.Button();
            ButtonPigeAuto = new System.Windows.Forms.Button();
            ButtonGetVersion = new System.Windows.Forms.Button();
            ButtonGetData = new System.Windows.Forms.Button();
            ButtonCreating = new System.Windows.Forms.Button();
            ButtonExit = new System.Windows.Forms.Button();
            StatusStrip1 = new System.Windows.Forms.StatusStrip();
            ToolStripProgressBar1 = new System.Windows.Forms.ToolStripProgressBar();
            ToolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            OpenFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            CheckBoxExe = new System.Windows.Forms.CheckBox();
            CheckBoxZip = new System.Windows.Forms.CheckBox();
            StatusStrip1.SuspendLayout();
            SuspendLayout();
            Label2.AutoSize = true;
            Label2.Location = new System.Drawing.Point(10, 39);
            Label2.Name = "Label2";
            Label2.Size = new System.Drawing.Size(108, 13);
            Label2.TabIndex = 13;
            Label2.Text = "Information Package:";
            Label1.AutoSize = true;
            Label1.Location = new System.Drawing.Point(10, 12);
            Label1.Name = "Label1";
            Label1.Size = new System.Drawing.Size(82, 13);
            Label1.TabIndex = 12;
            Label1.Text = "Package name:";
            ComboBoxPatchName.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            ComboBoxPatchName.FormattingEnabled = true;
            ComboBoxPatchName.Location = new System.Drawing.Point(124, 12);
            ComboBoxPatchName.Name = "ComboBoxPatchName";
            ComboBoxPatchName.Size = new System.Drawing.Size(248, 21);
            ComboBoxPatchName.TabIndex = 11;
            TextPatchInformation.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            TextPatchInformation.Location = new System.Drawing.Point(123, 39);
            TextPatchInformation.Multiline = true;
            TextPatchInformation.Name = "TextPatchInformation";
            TextPatchInformation.Size = new System.Drawing.Size(249, 193);
            TextPatchInformation.TabIndex = 10;
            OpenFileDialogVersion.FileName = "OpenFileDialog1";
            ButtonGetTarifs.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left;
            ButtonGetTarifs.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ButtonGetTarifs.Image = Resources.Tarifs;
            ButtonGetTarifs.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            ButtonGetTarifs.Location = new System.Drawing.Point(12, 103);
            ButtonGetTarifs.Name = "ButtonGetTarifs";
            ButtonGetTarifs.Size = new System.Drawing.Size(105, 39);
            ButtonGetTarifs.TabIndex = 23;
            ButtonGetTarifs.Text = "Tarifs     ";
            ButtonGetTarifs.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            ButtonGetTarifs.UseVisualStyleBackColor = true;
            ButtonPigeAuto.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left;
            ButtonPigeAuto.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ButtonPigeAuto.Image = Resources.Wizard_Wand;
            ButtonPigeAuto.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            ButtonPigeAuto.Location = new System.Drawing.Point(12, 58);
            ButtonPigeAuto.Name = "ButtonPigeAuto";
            ButtonPigeAuto.Size = new System.Drawing.Size(105, 39);
            ButtonPigeAuto.TabIndex = 22;
            ButtonPigeAuto.Text = "Pige Auto";
            ButtonPigeAuto.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            ButtonPigeAuto.UseVisualStyleBackColor = true;
            ButtonGetVersion.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left;
            ButtonGetVersion.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ButtonGetVersion.Image = Resources.Copy_to_Folder;
            ButtonGetVersion.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            ButtonGetVersion.Location = new System.Drawing.Point(12, 148);
            ButtonGetVersion.Name = "ButtonGetVersion";
            ButtonGetVersion.Size = new System.Drawing.Size(105, 39);
            ButtonGetVersion.TabIndex = 21;
            ButtonGetVersion.Text = "Version  ";
            ButtonGetVersion.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            ButtonGetVersion.UseVisualStyleBackColor = true;
            ButtonGetData.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left;
            ButtonGetData.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ButtonGetData.Image = Resources.ImportData;
            ButtonGetData.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            ButtonGetData.Location = new System.Drawing.Point(12, 193);
            ButtonGetData.Name = "ButtonGetData";
            ButtonGetData.Size = new System.Drawing.Size(105, 39);
            ButtonGetData.TabIndex = 17;
            ButtonGetData.Text = "Sources ";
            ButtonGetData.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            ButtonGetData.UseVisualStyleBackColor = true;
            ButtonCreating.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right;
            ButtonCreating.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ButtonCreating.Image = Resources.Package2;
            ButtonCreating.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            ButtonCreating.Location = new System.Drawing.Point(158, 238);
            ButtonCreating.Name = "ButtonCreating";
            ButtonCreating.Size = new System.Drawing.Size(104, 39);
            ButtonCreating.TabIndex = 14;
            ButtonCreating.Text = "Creating";
            ButtonCreating.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            ButtonCreating.UseVisualStyleBackColor = true;
            ButtonExit.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right;
            ButtonExit.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ButtonExit.Image = Resources.sortie;
            ButtonExit.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            ButtonExit.Location = new System.Drawing.Point(268, 238);
            ButtonExit.Name = "ButtonExit";
            ButtonExit.Size = new System.Drawing.Size(104, 39);
            ButtonExit.TabIndex = 9;
            ButtonExit.Text = "Exit    ";
            ButtonExit.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            ButtonExit.UseVisualStyleBackColor = true;
            StatusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[2] { ToolStripProgressBar1, ToolStripStatusLabel1 });
            StatusStrip1.Location = new System.Drawing.Point(0, 280);
            StatusStrip1.Name = "StatusStrip1";
            StatusStrip1.Size = new System.Drawing.Size(384, 22);
            StatusStrip1.TabIndex = 24;
            StatusStrip1.Text = "StatusStrip1";
            ToolStripProgressBar1.MarqueeAnimationSpeed = 50;
            ToolStripProgressBar1.Name = "ToolStripProgressBar1";
            ToolStripProgressBar1.Size = new System.Drawing.Size(100, 16);
            ToolStripStatusLabel1.Name = "ToolStripStatusLabel1";
            ToolStripStatusLabel1.Size = new System.Drawing.Size(0, 17);
            OpenFileDialog1.FileName = "OpenFileDialog1";
            CheckBoxExe.AutoSize = true;
            CheckBoxExe.Location = new System.Drawing.Point(12, 260);
            CheckBoxExe.Name = "CheckBoxExe";
            CheckBoxExe.Size = new System.Drawing.Size(96, 17);
            CheckBoxExe.TabIndex = 27;
            CheckBoxExe.Text = "EXE Extension";
            CheckBoxExe.UseVisualStyleBackColor = true;
            CheckBoxZip.AutoSize = true;
            CheckBoxZip.Location = new System.Drawing.Point(12, 237);
            CheckBoxZip.Name = "CheckBoxZip";
            CheckBoxZip.Size = new System.Drawing.Size(92, 17);
            CheckBoxZip.TabIndex = 28;
            CheckBoxZip.Text = "ZIP Extension";
            CheckBoxZip.UseVisualStyleBackColor = true;
            base.AutoScaleDimensions = new System.Drawing.SizeF(6f, 13f);
            base.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            base.ClientSize = new System.Drawing.Size(384, 302);
            base.Controls.Add(CheckBoxZip);
            base.Controls.Add(CheckBoxExe);
            base.Controls.Add(StatusStrip1);
            base.Controls.Add(ButtonGetTarifs);
            base.Controls.Add(ButtonPigeAuto);
            base.Controls.Add(ButtonGetVersion);
            base.Controls.Add(ButtonGetData);
            base.Controls.Add(ButtonCreating);
            base.Controls.Add(Label2);
            base.Controls.Add(Label1);
            base.Controls.Add(ComboBoxPatchName);
            base.Controls.Add(TextPatchInformation);
            base.Controls.Add(ButtonExit);
            //base.Icon = (System.Drawing.Icon)resources.GetObject("$this.Icon");
            base.KeyPreview = true;
            base.MaximizeBox = false;
            MinimumSize = new System.Drawing.Size(365, 340);
            base.Name = "CreateUpdate";
            base.ShowInTaskbar = false;
            base.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            Text = "Create Patch";
            StatusStrip1.ResumeLayout(false);
            StatusStrip1.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion


        private Button _ButtonCreating;
        private Label _Label2;
        private Label _Label1;
        private ComboBox _ComboBoxPatchName;
        private TextBox _TextPatchInformation;
        private Button _ButtonExit;
        private Button _ButtonGetData;
        private OpenFileDialog _OpenFileDialogVersion;
        private Button _ButtonGetVersion;
        private Button _ButtonPigeAuto;
        private Button _ButtonGetTarifs;
        private StatusStrip _StatusStrip1;
        private ToolStripProgressBar _ToolStripProgressBar1;
        private ToolStripStatusLabel _ToolStripStatusLabel1;
        private OpenFileDialog _OpenFileDialog1;
        private CheckBox _CheckBoxExe;
        private CheckBox _CheckBoxZip;

        internal virtual Button ButtonCreating
        {
            [CompilerGenerated]
            get
            {
                return _ButtonCreating;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = ButtonCreating_Click;
                Button buttonCreating = _ButtonCreating;
                if (buttonCreating != null)
                {
                    buttonCreating.Click -= value2;
                }
                _ButtonCreating = value;
                buttonCreating = _ButtonCreating;
                if (buttonCreating != null)
                {
                    buttonCreating.Click += value2;
                }
            }
        }

        [field: AccessedThroughProperty("Label2")]
        internal virtual Label Label2
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("Label1")]
        internal virtual Label Label1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual ComboBox ComboBoxPatchName
        {
            [CompilerGenerated]
            get
            {
                return _ComboBoxPatchName;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = ComboBox1_SelectedIndexChanged;
                ComboBox comboBoxPatchName = _ComboBoxPatchName;
                if (comboBoxPatchName != null)
                {
                    comboBoxPatchName.SelectedIndexChanged -= value2;
                }
                _ComboBoxPatchName = value;
                comboBoxPatchName = _ComboBoxPatchName;
                if (comboBoxPatchName != null)
                {
                    comboBoxPatchName.SelectedIndexChanged += value2;
                }
            }
        }

        [field: AccessedThroughProperty("TextPatchInformation")]
        internal virtual TextBox TextPatchInformation
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual Button ButtonExit
        {
            [CompilerGenerated]
            get
            {
                return _ButtonExit;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = ButtonExit_Click;
                Button buttonExit = _ButtonExit;
                if (buttonExit != null)
                {
                    buttonExit.Click -= value2;
                }
                _ButtonExit = value;
                buttonExit = _ButtonExit;
                if (buttonExit != null)
                {
                    buttonExit.Click += value2;
                }
            }
        }

        internal virtual Button ButtonGetData
        {
            [CompilerGenerated]
            get
            {
                return _ButtonGetData;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = ButtonGetData_Click;
                Button buttonGetData = _ButtonGetData;
                if (buttonGetData != null)
                {
                    buttonGetData.Click -= value2;
                }
                _ButtonGetData = value;
                buttonGetData = _ButtonGetData;
                if (buttonGetData != null)
                {
                    buttonGetData.Click += value2;
                }
            }
        }

        [field: AccessedThroughProperty("OpenFileDialogVersion")]
        internal virtual OpenFileDialog OpenFileDialogVersion
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual Button ButtonGetVersion
        {
            [CompilerGenerated]
            get
            {
                return _ButtonGetVersion;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = ButtonGetVersion_Click;
                Button buttonGetVersion = _ButtonGetVersion;
                if (buttonGetVersion != null)
                {
                    buttonGetVersion.Click -= value2;
                }
                _ButtonGetVersion = value;
                buttonGetVersion = _ButtonGetVersion;
                if (buttonGetVersion != null)
                {
                    buttonGetVersion.Click += value2;
                }
            }
        }

        internal virtual Button ButtonPigeAuto
        {
            [CompilerGenerated]
            get
            {
                return _ButtonPigeAuto;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = ButtonPigeAuto_Click;
                Button buttonPigeAuto = _ButtonPigeAuto;
                if (buttonPigeAuto != null)
                {
                    buttonPigeAuto.Click -= value2;
                }
                _ButtonPigeAuto = value;
                buttonPigeAuto = _ButtonPigeAuto;
                if (buttonPigeAuto != null)
                {
                    buttonPigeAuto.Click += value2;
                }
            }
        }

        internal virtual Button ButtonGetTarifs
        {
            [CompilerGenerated]
            get
            {
                return _ButtonGetTarifs;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = ButtonGetTarifs_Click;
                Button buttonGetTarifs = _ButtonGetTarifs;
                if (buttonGetTarifs != null)
                {
                    buttonGetTarifs.Click -= value2;
                }
                _ButtonGetTarifs = value;
                buttonGetTarifs = _ButtonGetTarifs;
                if (buttonGetTarifs != null)
                {
                    buttonGetTarifs.Click += value2;
                }
            }
        }

        [field: AccessedThroughProperty("StatusStrip1")]
        internal virtual StatusStrip StatusStrip1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("ToolStripProgressBar1")]
        internal virtual ToolStripProgressBar ToolStripProgressBar1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("ToolStripStatusLabel1")]
        internal virtual ToolStripStatusLabel ToolStripStatusLabel1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("OpenFileDialog1")]
        internal virtual OpenFileDialog OpenFileDialog1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("CheckBoxExe")]
        internal virtual CheckBox CheckBoxExe
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("CheckBoxZip")]
        internal virtual CheckBox CheckBoxZip
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }
    }
}