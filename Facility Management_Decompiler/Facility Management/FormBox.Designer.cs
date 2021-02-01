
using System;
using System.ComponentModel;
using System.Drawing;
using System.Runtime.CompilerServices;
using System.Windows.Forms;

namespace Facility_Management
{
    partial class FormBox
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

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        [System.Diagnostics.DebuggerStepThrough]
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
            //System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Facility_Management.FormBox));
            CMS_ListPatch = new System.Windows.Forms.ContextMenuStrip(components);
            CMS_ListSelectClient = new System.Windows.Forms.ContextMenuStrip(components);
            CMS_ListSelectPatch = new System.Windows.Forms.ContextMenuStrip(components);
            CMS_ListAccesClient = new System.Windows.Forms.ContextMenuStrip(components);
            MenuStripFM = new System.Windows.Forms.MenuStrip();
            StatusStripFM = new System.Windows.Forms.StatusStrip();
            ToolStripProgressBar1 = new System.Windows.Forms.ToolStripProgressBar();
            ToolStripStatusLabelFM = new System.Windows.Forms.ToolStripStatusLabel();
            ToolStripStatusLabelDisk = new System.Windows.Forms.ToolStripStatusLabel();
            ToolTip1 = new System.Windows.Forms.ToolTip(components);
            Label_version = new System.Windows.Forms.Label();
            ListSelectClient = new System.Windows.Forms.ListBox();
            Label1 = new System.Windows.Forms.Label();
            ListAccesClient = new System.Windows.Forms.ListBox();
            Label2 = new System.Windows.Forms.Label();
            Label3 = new System.Windows.Forms.Label();
            ListSelectPatch = new System.Windows.Forms.ListBox();
            SplitContainer1 = new System.Windows.Forms.SplitContainer();
            SplitContainer2 = new System.Windows.Forms.SplitContainer();
            SplitContainer3 = new System.Windows.Forms.SplitContainer();
            SplitContainer4 = new System.Windows.Forms.SplitContainer();
            ButtonFiltreUpdates = new System.Windows.Forms.Button();
            ListPatch = new System.Windows.Forms.ListBox();
            FiltrePatch = new System.Windows.Forms.TextBox();
            Label5 = new System.Windows.Forms.Label();
            Label4 = new System.Windows.Forms.Label();
            SplitContainer5 = new System.Windows.Forms.SplitContainer();
            SplitContainer7 = new System.Windows.Forms.SplitContainer();
            SplitContainer8 = new System.Windows.Forms.SplitContainer();
            F2 = new System.Windows.Forms.Button();
            F3 = new System.Windows.Forms.Button();
            SplitContainer9 = new System.Windows.Forms.SplitContainer();
            F4 = new System.Windows.Forms.Button();
            F5 = new System.Windows.Forms.Button();
            SplitContainer6 = new System.Windows.Forms.SplitContainer();
            SplitContainer10 = new System.Windows.Forms.SplitContainer();
            F6 = new System.Windows.Forms.Button();
            F9 = new System.Windows.Forms.Button();
            F10 = new System.Windows.Forms.Button();
            FolderBrowserDialogZip = new System.Windows.Forms.FolderBrowserDialog();
            OpenFilePatchList = new System.Windows.Forms.OpenFileDialog();
            StatusStripFM.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer1).BeginInit();
            SplitContainer1.Panel1.SuspendLayout();
            SplitContainer1.Panel2.SuspendLayout();
            SplitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer2).BeginInit();
            SplitContainer2.Panel1.SuspendLayout();
            SplitContainer2.Panel2.SuspendLayout();
            SplitContainer2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer3).BeginInit();
            SplitContainer3.Panel1.SuspendLayout();
            SplitContainer3.Panel2.SuspendLayout();
            SplitContainer3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer4).BeginInit();
            SplitContainer4.Panel1.SuspendLayout();
            SplitContainer4.Panel2.SuspendLayout();
            SplitContainer4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer5).BeginInit();
            SplitContainer5.Panel1.SuspendLayout();
            SplitContainer5.Panel2.SuspendLayout();
            SplitContainer5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer7).BeginInit();
            SplitContainer7.Panel1.SuspendLayout();
            SplitContainer7.Panel2.SuspendLayout();
            SplitContainer7.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer8).BeginInit();
            SplitContainer8.Panel1.SuspendLayout();
            SplitContainer8.Panel2.SuspendLayout();
            SplitContainer8.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer9).BeginInit();
            SplitContainer9.Panel1.SuspendLayout();
            SplitContainer9.Panel2.SuspendLayout();
            SplitContainer9.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer6).BeginInit();
            SplitContainer6.Panel1.SuspendLayout();
            SplitContainer6.Panel2.SuspendLayout();
            SplitContainer6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer10).BeginInit();
            SplitContainer10.Panel1.SuspendLayout();
            SplitContainer10.Panel2.SuspendLayout();
            SplitContainer10.SuspendLayout();
            SuspendLayout();
            CMS_ListPatch.Name = "CMS_ListPatch";
            CMS_ListPatch.Size = new System.Drawing.Size(61, 4);
            CMS_ListSelectClient.Name = "CMS_ListSelectClient";
            CMS_ListSelectClient.Size = new System.Drawing.Size(61, 4);
            CMS_ListSelectPatch.Name = "CMS_ListSelectPatch";
            CMS_ListSelectPatch.Size = new System.Drawing.Size(61, 4);
            CMS_ListAccesClient.Name = "CMS_ListAccesClient";
            CMS_ListAccesClient.Size = new System.Drawing.Size(61, 4);
            MenuStripFM.AutoSize = false;
            MenuStripFM.GripMargin = new System.Windows.Forms.Padding(0);
            MenuStripFM.Location = new System.Drawing.Point(0, 0);
            MenuStripFM.Name = "MenuStripFM";
            MenuStripFM.Padding = new System.Windows.Forms.Padding(0, 2, 0, 2);
            MenuStripFM.Size = new System.Drawing.Size(776, 40);
            MenuStripFM.TabIndex = 21;
            MenuStripFM.Text = "MenuStrip1";
            StatusStripFM.Items.AddRange(new System.Windows.Forms.ToolStripItem[3] { ToolStripProgressBar1, ToolStripStatusLabelFM, ToolStripStatusLabelDisk });
            StatusStripFM.Location = new System.Drawing.Point(0, 439);
            StatusStripFM.Name = "StatusStripFM";
            StatusStripFM.Size = new System.Drawing.Size(776, 22);
            StatusStripFM.TabIndex = 22;
            StatusStripFM.Text = "StatusStrip1";
            ToolStripProgressBar1.Name = "ToolStripProgressBar1";
            ToolStripProgressBar1.Size = new System.Drawing.Size(150, 16);
            ToolStripProgressBar1.Step = 1;
            ToolStripProgressBar1.Style = System.Windows.Forms.ProgressBarStyle.Continuous;
            ToolStripStatusLabelFM.Name = "ToolStripStatusLabelFM";
            ToolStripStatusLabelFM.Size = new System.Drawing.Size(130, 17);
            ToolStripStatusLabelFM.Text = "ToolStripStatusLabelFM";
            ToolStripStatusLabelDisk.Name = "ToolStripStatusLabelDisk";
            ToolStripStatusLabelDisk.Size = new System.Drawing.Size(135, 17);
            ToolStripStatusLabelDisk.Text = "ToolStripStatusLabelDisk";
            Label_version.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right;
            Label_version.AutoSize = true;
            Label_version.Location = new System.Drawing.Point(726, 444);
            Label_version.Name = "Label_version";
            Label_version.Size = new System.Drawing.Size(46, 13);
            Label_version.TabIndex = 24;
            Label_version.Text = "v1.0.0.0";
            ListSelectClient.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            ListSelectClient.BorderStyle = System.Windows.Forms.BorderStyle.None;
            ListSelectClient.ContextMenuStrip = CMS_ListSelectClient;
            ListSelectClient.Cursor = System.Windows.Forms.Cursors.Default;
            ListSelectClient.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ListSelectClient.FormattingEnabled = true;
            ListSelectClient.IntegralHeight = false;
            ListSelectClient.ItemHeight = 16;
            ListSelectClient.Location = new System.Drawing.Point(3, 21);
            ListSelectClient.Name = "ListSelectClient";
            ListSelectClient.Size = new System.Drawing.Size(181, 331);
            ListSelectClient.TabIndex = 25;
            Label1.AutoSize = true;
            Label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f);
            Label1.Location = new System.Drawing.Point(3, 0);
            Label1.Name = "Label1";
            Label1.Size = new System.Drawing.Size(105, 18);
            Label1.TabIndex = 26;
            Label1.Text = "Customer List:";
            ListAccesClient.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            ListAccesClient.BackColor = System.Drawing.SystemColors.Window;
            ListAccesClient.BorderStyle = System.Windows.Forms.BorderStyle.None;
            ListAccesClient.ContextMenuStrip = CMS_ListAccesClient;
            ListAccesClient.Cursor = System.Windows.Forms.Cursors.Default;
            ListAccesClient.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ListAccesClient.FormattingEnabled = true;
            ListAccesClient.IntegralHeight = false;
            ListAccesClient.ItemHeight = 16;
            ListAccesClient.Location = new System.Drawing.Point(3, 21);
            ListAccesClient.Name = "ListAccesClient";
            ListAccesClient.Size = new System.Drawing.Size(179, 331);
            ListAccesClient.TabIndex = 25;
            Label2.AutoSize = true;
            Label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f);
            Label2.Location = new System.Drawing.Point(3, 0);
            Label2.Name = "Label2";
            Label2.Size = new System.Drawing.Size(144, 18);
            Label2.TabIndex = 27;
            Label2.Text = "Selected customers:";
            Label3.AutoSize = true;
            Label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f);
            Label3.Location = new System.Drawing.Point(3, 0);
            Label3.Name = "Label3";
            Label3.Size = new System.Drawing.Size(128, 18);
            Label3.TabIndex = 28;
            Label3.Text = "Selected Updates:";
            ListSelectPatch.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            ListSelectPatch.BorderStyle = System.Windows.Forms.BorderStyle.None;
            ListSelectPatch.ContextMenuStrip = CMS_ListSelectPatch;
            ListSelectPatch.Cursor = System.Windows.Forms.Cursors.Default;
            ListSelectPatch.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ListSelectPatch.FormattingEnabled = true;
            ListSelectPatch.IntegralHeight = false;
            ListSelectPatch.ItemHeight = 16;
            ListSelectPatch.Location = new System.Drawing.Point(3, 21);
            ListSelectPatch.Name = "ListSelectPatch";
            ListSelectPatch.Size = new System.Drawing.Size(178, 331);
            ListSelectPatch.TabIndex = 29;
            SplitContainer1.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            SplitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel2;
            SplitContainer1.IsSplitterFixed = true;
            SplitContainer1.Location = new System.Drawing.Point(12, 43);
            SplitContainer1.Name = "SplitContainer1";
            SplitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal;
            SplitContainer1.Panel1.Controls.Add(SplitContainer2);
            SplitContainer1.Panel2.Controls.Add(SplitContainer5);
            SplitContainer1.Size = new System.Drawing.Size(752, 393);
            SplitContainer1.SplitterDistance = 355;
            SplitContainer1.TabIndex = 35;
            SplitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            SplitContainer2.Location = new System.Drawing.Point(0, 0);
            SplitContainer2.Name = "SplitContainer2";
            SplitContainer2.Panel1.Controls.Add(SplitContainer3);
            SplitContainer2.Panel2.Controls.Add(SplitContainer4);
            SplitContainer2.Size = new System.Drawing.Size(752, 355);
            SplitContainer2.SplitterDistance = 376;
            SplitContainer2.TabIndex = 0;
            SplitContainer3.Dock = System.Windows.Forms.DockStyle.Fill;
            SplitContainer3.Location = new System.Drawing.Point(0, 0);
            SplitContainer3.Name = "SplitContainer3";
            SplitContainer3.Panel1.Controls.Add(ListAccesClient);
            SplitContainer3.Panel1.Controls.Add(Label1);
            SplitContainer3.Panel2.Controls.Add(ListSelectClient);
            SplitContainer3.Panel2.Controls.Add(Label2);
            SplitContainer3.Size = new System.Drawing.Size(376, 355);
            SplitContainer3.SplitterDistance = 185;
            SplitContainer3.TabIndex = 0;
            SplitContainer4.Dock = System.Windows.Forms.DockStyle.Fill;
            SplitContainer4.Location = new System.Drawing.Point(0, 0);
            SplitContainer4.Name = "SplitContainer4";
            SplitContainer4.Panel1.Controls.Add(Label3);
            SplitContainer4.Panel1.Controls.Add(ListSelectPatch);
            SplitContainer4.Panel2.Controls.Add(ButtonFiltreUpdates);
            SplitContainer4.Panel2.Controls.Add(ListPatch);
            SplitContainer4.Panel2.Controls.Add(FiltrePatch);
            SplitContainer4.Panel2.Controls.Add(Label5);
            SplitContainer4.Panel2.Controls.Add(Label4);
            SplitContainer4.Size = new System.Drawing.Size(372, 355);
            SplitContainer4.SplitterDistance = 184;
            SplitContainer4.TabIndex = 0;
            ButtonFiltreUpdates.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right;
            ButtonFiltreUpdates.FlatAppearance.BorderSize = 0;
            ButtonFiltreUpdates.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            ButtonFiltreUpdates.Location = new System.Drawing.Point(160, 325);
            ButtonFiltreUpdates.Name = "ButtonFiltreUpdates";
            ButtonFiltreUpdates.Size = new System.Drawing.Size(21, 27);
            ButtonFiltreUpdates.TabIndex = 34;
            ButtonFiltreUpdates.Text = "X";
            ButtonFiltreUpdates.UseVisualStyleBackColor = true;
            ListPatch.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            ListPatch.BorderStyle = System.Windows.Forms.BorderStyle.None;
            ListPatch.ContextMenuStrip = CMS_ListPatch;
            ListPatch.Cursor = System.Windows.Forms.Cursors.Default;
            ListPatch.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ListPatch.FormattingEnabled = true;
            ListPatch.IntegralHeight = false;
            ListPatch.ItemHeight = 16;
            ListPatch.Location = new System.Drawing.Point(3, 21);
            ListPatch.Name = "ListPatch";
            ListPatch.Size = new System.Drawing.Size(178, 292);
            ListPatch.TabIndex = 30;
            FiltrePatch.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            FiltrePatch.BorderStyle = System.Windows.Forms.BorderStyle.None;
            FiltrePatch.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            FiltrePatch.Location = new System.Drawing.Point(3, 337);
            FiltrePatch.Name = "FiltrePatch";
            FiltrePatch.Size = new System.Drawing.Size(151, 15);
            FiltrePatch.TabIndex = 33;
            Label5.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left;
            Label5.AutoSize = true;
            Label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f);
            Label5.Location = new System.Drawing.Point(3, 316);
            Label5.Name = "Label5";
            Label5.Size = new System.Drawing.Size(99, 18);
            Label5.TabIndex = 32;
            Label5.Text = "Filter Updates";
            Label4.AutoSize = true;
            Label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f);
            Label4.Location = new System.Drawing.Point(3, 0);
            Label4.Name = "Label4";
            Label4.Size = new System.Drawing.Size(94, 18);
            Label4.TabIndex = 31;
            Label4.Text = "Updates List:";
            SplitContainer5.Dock = System.Windows.Forms.DockStyle.Fill;
            SplitContainer5.IsSplitterFixed = true;
            SplitContainer5.Location = new System.Drawing.Point(0, 0);
            SplitContainer5.Name = "SplitContainer5";
            SplitContainer5.Panel1.Controls.Add(SplitContainer7);
            SplitContainer5.Panel2.Controls.Add(SplitContainer6);
            SplitContainer5.Size = new System.Drawing.Size(752, 34);
            SplitContainer5.SplitterDistance = 429;
            SplitContainer5.TabIndex = 0;
            SplitContainer7.Dock = System.Windows.Forms.DockStyle.Fill;
            SplitContainer7.IsSplitterFixed = true;
            SplitContainer7.Location = new System.Drawing.Point(0, 0);
            SplitContainer7.Name = "SplitContainer7";
            SplitContainer7.Panel1.Controls.Add(SplitContainer8);
            SplitContainer7.Panel2.Controls.Add(SplitContainer9);
            SplitContainer7.Size = new System.Drawing.Size(429, 34);
            SplitContainer7.SplitterDistance = 214;
            SplitContainer7.TabIndex = 0;
            SplitContainer8.Dock = System.Windows.Forms.DockStyle.Fill;
            SplitContainer8.IsSplitterFixed = true;
            SplitContainer8.Location = new System.Drawing.Point(0, 0);
            SplitContainer8.Name = "SplitContainer8";
            SplitContainer8.Panel1.Controls.Add(F2);
            SplitContainer8.Panel2.Controls.Add(F3);
            SplitContainer8.Size = new System.Drawing.Size(214, 34);
            SplitContainer8.SplitterDistance = 107;
            SplitContainer8.TabIndex = 0;
            F2.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            F2.AutoSize = true;
            F2.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            F2.Location = new System.Drawing.Point(3, 3);
            F2.Name = "F2";
            F2.Size = new System.Drawing.Size(101, 28);
            F2.TabIndex = 35;
            F2.Text = "F2";
            F2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            F2.UseVisualStyleBackColor = true;
            F3.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            F3.AutoSize = true;
            F3.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            F3.Location = new System.Drawing.Point(2, 3);
            F3.Name = "F3";
            F3.Size = new System.Drawing.Size(98, 28);
            F3.TabIndex = 36;
            F3.Text = "F3";
            F3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            F3.UseVisualStyleBackColor = true;
            SplitContainer9.Dock = System.Windows.Forms.DockStyle.Fill;
            SplitContainer9.IsSplitterFixed = true;
            SplitContainer9.Location = new System.Drawing.Point(0, 0);
            SplitContainer9.Name = "SplitContainer9";
            SplitContainer9.Panel1.Controls.Add(F4);
            SplitContainer9.Panel2.Controls.Add(F5);
            SplitContainer9.Size = new System.Drawing.Size(211, 34);
            SplitContainer9.SplitterDistance = 105;
            SplitContainer9.TabIndex = 0;
            F4.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            F4.AutoSize = true;
            F4.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            F4.Location = new System.Drawing.Point(3, 3);
            F4.Name = "F4";
            F4.Size = new System.Drawing.Size(99, 28);
            F4.TabIndex = 37;
            F4.Text = "F4";
            F4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            F4.UseVisualStyleBackColor = true;
            F5.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            F5.AutoSize = true;
            F5.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            F5.Location = new System.Drawing.Point(3, 3);
            F5.Name = "F5";
            F5.Size = new System.Drawing.Size(96, 28);
            F5.TabIndex = 38;
            F5.Text = "F5";
            F5.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            F5.UseVisualStyleBackColor = true;
            SplitContainer6.Dock = System.Windows.Forms.DockStyle.Fill;
            SplitContainer6.IsSplitterFixed = true;
            SplitContainer6.Location = new System.Drawing.Point(0, 0);
            SplitContainer6.Name = "SplitContainer6";
            SplitContainer6.Panel1.Controls.Add(SplitContainer10);
            SplitContainer6.Panel2.Controls.Add(F10);
            SplitContainer6.Size = new System.Drawing.Size(319, 34);
            SplitContainer6.SplitterDistance = 212;
            SplitContainer6.TabIndex = 0;
            SplitContainer10.Dock = System.Windows.Forms.DockStyle.Fill;
            SplitContainer10.IsSplitterFixed = true;
            SplitContainer10.Location = new System.Drawing.Point(0, 0);
            SplitContainer10.Name = "SplitContainer10";
            SplitContainer10.Panel1.Controls.Add(F6);
            SplitContainer10.Panel2.Controls.Add(F9);
            SplitContainer10.Size = new System.Drawing.Size(212, 34);
            SplitContainer10.SplitterDistance = 106;
            SplitContainer10.TabIndex = 0;
            F6.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            F6.AutoSize = true;
            F6.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            F6.Location = new System.Drawing.Point(3, 3);
            F6.Name = "F6";
            F6.Size = new System.Drawing.Size(100, 28);
            F6.TabIndex = 39;
            F6.Text = "F6";
            F6.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            F6.UseVisualStyleBackColor = true;
            F9.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            F9.AutoSize = true;
            F9.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            F9.Location = new System.Drawing.Point(3, 3);
            F9.Name = "F9";
            F9.Size = new System.Drawing.Size(96, 28);
            F9.TabIndex = 40;
            F9.Text = "F9";
            F9.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            F9.UseVisualStyleBackColor = true;
            F10.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            F10.AutoSize = true;
            F10.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            F10.Location = new System.Drawing.Point(3, 3);
            F10.Name = "F10";
            F10.Size = new System.Drawing.Size(97, 28);
            F10.TabIndex = 41;
            F10.Text = "F10";
            F10.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            F10.UseVisualStyleBackColor = true;
            OpenFilePatchList.FileName = "OpenFilePatchList";
            base.AutoScaleDimensions = new System.Drawing.SizeF(6f, 13f);
            base.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            base.ClientSize = new System.Drawing.Size(776, 461);
            base.Controls.Add(SplitContainer1);
            base.Controls.Add(Label_version);
            base.Controls.Add(StatusStripFM);
            base.Controls.Add(MenuStripFM);
            //base.Icon = (System.Drawing.Icon)resources.GetObject("$this.Icon");
            base.KeyPreview = true;
            base.MainMenuStrip = MenuStripFM;
            MinimumSize = new System.Drawing.Size(792, 500);
            base.Name = "FormBox";
            base.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            Text = "Facility Management";
            StatusStripFM.ResumeLayout(false);
            StatusStripFM.PerformLayout();
            SplitContainer1.Panel1.ResumeLayout(false);
            SplitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)SplitContainer1).EndInit();
            SplitContainer1.ResumeLayout(false);
            SplitContainer2.Panel1.ResumeLayout(false);
            SplitContainer2.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)SplitContainer2).EndInit();
            SplitContainer2.ResumeLayout(false);
            SplitContainer3.Panel1.ResumeLayout(false);
            SplitContainer3.Panel1.PerformLayout();
            SplitContainer3.Panel2.ResumeLayout(false);
            SplitContainer3.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer3).EndInit();
            SplitContainer3.ResumeLayout(false);
            SplitContainer4.Panel1.ResumeLayout(false);
            SplitContainer4.Panel1.PerformLayout();
            SplitContainer4.Panel2.ResumeLayout(false);
            SplitContainer4.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer4).EndInit();
            SplitContainer4.ResumeLayout(false);
            SplitContainer5.Panel1.ResumeLayout(false);
            SplitContainer5.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)SplitContainer5).EndInit();
            SplitContainer5.ResumeLayout(false);
            SplitContainer7.Panel1.ResumeLayout(false);
            SplitContainer7.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)SplitContainer7).EndInit();
            SplitContainer7.ResumeLayout(false);
            SplitContainer8.Panel1.ResumeLayout(false);
            SplitContainer8.Panel1.PerformLayout();
            SplitContainer8.Panel2.ResumeLayout(false);
            SplitContainer8.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer8).EndInit();
            SplitContainer8.ResumeLayout(false);
            SplitContainer9.Panel1.ResumeLayout(false);
            SplitContainer9.Panel1.PerformLayout();
            SplitContainer9.Panel2.ResumeLayout(false);
            SplitContainer9.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer9).EndInit();
            SplitContainer9.ResumeLayout(false);
            SplitContainer6.Panel1.ResumeLayout(false);
            SplitContainer6.Panel2.ResumeLayout(false);
            SplitContainer6.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer6).EndInit();
            SplitContainer6.ResumeLayout(false);
            SplitContainer10.Panel1.ResumeLayout(false);
            SplitContainer10.Panel1.PerformLayout();
            SplitContainer10.Panel2.ResumeLayout(false);
            SplitContainer10.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer10).EndInit();
            SplitContainer10.ResumeLayout(false);
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        [CompilerGenerated]
        [AccessedThroughProperty("SoftwareMenu")]
        private ToolStripMenuItem _SoftwareMenu;

        [CompilerGenerated]
        [AccessedThroughProperty("ExitMenu")]
        private ToolStripMenuItem _ExitMenu;

        [CompilerGenerated]
        [AccessedThroughProperty("CountryMenu")]
        private ToolStripMenuItem _CountryMenu;

        [CompilerGenerated]
        [AccessedThroughProperty("CustomerMenu")]
        private ToolStripMenuItem _CustomerMenu;

        [CompilerGenerated]
        [AccessedThroughProperty("ToolsMenu")]
        private ToolStripMenuItem _ToolsMenu;

        [CompilerGenerated]
        [AccessedThroughProperty("UpdateMenu")]
        private ToolStripMenuItem _UpdateMenu;

        [CompilerGenerated]
        [AccessedThroughProperty("UnderMenuService")]
        private ToolStripMenuItem _UnderMenuService;

        [CompilerGenerated]
        [AccessedThroughProperty("AddMenuService")]
        private ToolStripMenuItem _AddMenuService;

        [CompilerGenerated]
        [AccessedThroughProperty("DelMenuService")]
        private ToolStripMenuItem _DelMenuService;

        [CompilerGenerated]
        [AccessedThroughProperty("SearchMenuService")]
        private ToolStripMenuItem _SearchMenuService;

        [CompilerGenerated]
        [AccessedThroughProperty("Group")]
        private ToolStripMenuItem _Group;

        [CompilerGenerated]
        [AccessedThroughProperty("Customer")]
        private ToolStripMenuItem _Customer;

        [CompilerGenerated]
        [AccessedThroughProperty("AliasCustomer")]
        private ToolStripMenuItem _AliasCustomer;
        
        [CompilerGenerated]
        [AccessedThroughProperty("MenuStripFM")]
        private MenuStrip _MenuStripFM;

        [CompilerGenerated]
        [AccessedThroughProperty("CMS_ListAccesClient")]
        private ContextMenuStrip _CMS_ListAccesClient;

        [CompilerGenerated]
        [AccessedThroughProperty("StatusStripFM")]
        private StatusStrip _StatusStripFM;

        [CompilerGenerated]
        [AccessedThroughProperty("ToolStripProgressBar1")]
        private ToolStripProgressBar _ToolStripProgressBar1;

        [CompilerGenerated]
        [AccessedThroughProperty("CMS_ListPatch")]
        private ContextMenuStrip _CMS_ListPatch;

        [CompilerGenerated]
        [AccessedThroughProperty("CMS_ListSelectPatch")]
        private ContextMenuStrip _CMS_ListSelectPatch;

        [CompilerGenerated]
        [AccessedThroughProperty("CMS_ListSelectClient")]
        private ContextMenuStrip _CMS_ListSelectClient;

        [CompilerGenerated]
        [AccessedThroughProperty("ToolStripStatusLabelFM")]
        private ToolStripStatusLabel _ToolStripStatusLabelFM;

        [CompilerGenerated]
        [AccessedThroughProperty("ToolTip1")]
        private ToolTip _ToolTip1;

        [CompilerGenerated]
        [AccessedThroughProperty("ToolStripStatusLabelDisk")]
        private ToolStripStatusLabel _ToolStripStatusLabelDisk;

        [CompilerGenerated]
        [AccessedThroughProperty("Label_version")]
        private Label _Label_version;

        [CompilerGenerated]
        [AccessedThroughProperty("ListSelectClient")]
        private ListBox _ListSelectClient;

        [CompilerGenerated]
        [AccessedThroughProperty("Label1")]
        private Label _Label1;

        [CompilerGenerated]
        [AccessedThroughProperty("ListAccesClient")]
        private ListBox _ListAccesClient;

        [CompilerGenerated]
        [AccessedThroughProperty("Label2")]
        private Label _Label2;

        [CompilerGenerated]
        [AccessedThroughProperty("Label3")]
        private Label _Label3;

        [CompilerGenerated]
        [AccessedThroughProperty("ListSelectPatch")]
        private ListBox _ListSelectPatch;

        [CompilerGenerated]
        [AccessedThroughProperty("SplitContainer1")]
        private SplitContainer _SplitContainer1;

        [CompilerGenerated]
        [AccessedThroughProperty("SplitContainer2")]
        private SplitContainer _SplitContainer2;

        [CompilerGenerated]
        [AccessedThroughProperty("SplitContainer3")]
        private SplitContainer _SplitContainer3;

        [CompilerGenerated]
        [AccessedThroughProperty("SplitContainer4")]
        private SplitContainer _SplitContainer4;

        [CompilerGenerated]
        [AccessedThroughProperty("ButtonFiltreUpdates")]
        private Button _ButtonFiltreUpdates;

        [CompilerGenerated]
        [AccessedThroughProperty("ListPatch")]
        private ListBox _ListPatch;

        [CompilerGenerated]
        [AccessedThroughProperty("FiltrePatch")]
        private TextBox _FiltrePatch;

        [CompilerGenerated]
        [AccessedThroughProperty("Label5")]
        private Label _Label5;

        [CompilerGenerated]
        [AccessedThroughProperty("Label4")]
        private Label _Label4;

        [CompilerGenerated]
        [AccessedThroughProperty("SplitContainer5")]
        private SplitContainer _SplitContainer5;

        [CompilerGenerated]
        [AccessedThroughProperty("SplitContainer7")]
        private SplitContainer _SplitContainer7;

        [CompilerGenerated]
        [AccessedThroughProperty("SplitContainer8")]
        private SplitContainer _SplitContainer8;

        [CompilerGenerated]
        [AccessedThroughProperty("F2")]
        private Button _F2;

        [CompilerGenerated]
        [AccessedThroughProperty("F3")]
        private Button _F3;

        [CompilerGenerated]
        [AccessedThroughProperty("SplitContainer9")]
        private SplitContainer _SplitContainer9;

        [CompilerGenerated]
        [AccessedThroughProperty("F4")]
        private Button _F4;

        [CompilerGenerated]
        [AccessedThroughProperty("F5")]
        private Button _F5;

        [CompilerGenerated]
        [AccessedThroughProperty("SplitContainer6")]
        private SplitContainer _SplitContainer6;

        [CompilerGenerated]
        [AccessedThroughProperty("SplitContainer10")]
        private SplitContainer _SplitContainer10;

        [CompilerGenerated]
        [AccessedThroughProperty("F6")]
        private Button _F6;

        [CompilerGenerated]
        [AccessedThroughProperty("F9")]
        private Button _F9;

        [CompilerGenerated]
        [AccessedThroughProperty("F10")]
        private Button _F10;

        [CompilerGenerated]
        [AccessedThroughProperty("FolderBrowserDialogZip")]
        private FolderBrowserDialog _FolderBrowserDialogZip;

        [CompilerGenerated]
        [AccessedThroughProperty("OpenFilePatchList")]
        private OpenFileDialog _OpenFilePatchList;

        internal virtual ToolStripMenuItem SoftwareMenu
        {
            [CompilerGenerated]
            get
            {
                return _SoftwareMenu;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ToolStripItemClickedEventHandler value2 = SoftwareMenu_DropDownItemClicked;
                EventHandler value3 = SoftwareMenu_DropDownOpening;
                ToolStripMenuItem softwareMenu = _SoftwareMenu;
                if (softwareMenu != null)
                {
                    softwareMenu.DropDownItemClicked -= value2;
                    softwareMenu.DropDownOpening -= value3;
                }
                _SoftwareMenu = value;
                softwareMenu = _SoftwareMenu;
                if (softwareMenu != null)
                {
                    softwareMenu.DropDownItemClicked += value2;
                    softwareMenu.DropDownOpening += value3;
                }
            }
        }

        internal virtual ToolStripMenuItem ExitMenu
        {
            [CompilerGenerated]
            get
            {
                return _ExitMenu;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = ExitMenu_Click;
                ToolStripMenuItem exitMenu = _ExitMenu;
                if (exitMenu != null)
                {
                    exitMenu.Click -= value2;
                }
                _ExitMenu = value;
                exitMenu = _ExitMenu;
                if (exitMenu != null)
                {
                    exitMenu.Click += value2;
                }
            }
        }

        internal virtual ToolStripMenuItem CountryMenu
        {
            [CompilerGenerated]
            get
            {
                return _CountryMenu;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ToolStripItemClickedEventHandler value2 = CountryMenu_DropDownItemClicked;
                ToolStripMenuItem countryMenu = _CountryMenu;
                if (countryMenu != null)
                {
                    countryMenu.DropDownItemClicked -= value2;
                }
                _CountryMenu = value;
                countryMenu = _CountryMenu;
                if (countryMenu != null)
                {
                    countryMenu.DropDownItemClicked += value2;
                }
            }
        }

        internal virtual ToolStripMenuItem CustomerMenu
        {
            [CompilerGenerated]
            get
            {
                return _CustomerMenu;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ToolStripItemClickedEventHandler value2 = CustomerMenu_DropDownItemClicked;
                ToolStripMenuItem customerMenu = _CustomerMenu;
                if (customerMenu != null)
                {
                    customerMenu.DropDownItemClicked -= value2;
                }
                _CustomerMenu = value;
                customerMenu = _CustomerMenu;
                if (customerMenu != null)
                {
                    customerMenu.DropDownItemClicked += value2;
                }
            }
        }

        internal virtual ToolStripMenuItem ToolsMenu
        {
            [CompilerGenerated]
            get
            {
                return _ToolsMenu;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ToolStripItemClickedEventHandler value2 = ToolsMenu_DropDownItemClicked;
                ToolStripMenuItem toolsMenu = _ToolsMenu;
                if (toolsMenu != null)
                {
                    toolsMenu.DropDownItemClicked -= value2;
                }
                _ToolsMenu = value;
                toolsMenu = _ToolsMenu;
                if (toolsMenu != null)
                {
                    toolsMenu.DropDownItemClicked += value2;
                }
            }
        }

        internal virtual ToolStripMenuItem UpdateMenu
        {
            [CompilerGenerated]
            get
            {
                return _UpdateMenu;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ToolStripItemClickedEventHandler value2 = UpdateMenu_DropDownItemClicked;
                ToolStripMenuItem updateMenu = _UpdateMenu;
                if (updateMenu != null)
                {
                    updateMenu.DropDownItemClicked -= value2;
                }
                _UpdateMenu = value;
                updateMenu = _UpdateMenu;
                if (updateMenu != null)
                {
                    updateMenu.DropDownItemClicked += value2;
                }
            }
        }

        [field: AccessedThroughProperty("UnderMenuService")]
        internal virtual ToolStripMenuItem UnderMenuService
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual ToolStripMenuItem AddMenuService
        {
            [CompilerGenerated]
            get
            {
                return _AddMenuService;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ToolStripItemClickedEventHandler value2 = AddMenuService_DropDownItemClicked;
                ToolStripMenuItem addMenuService = _AddMenuService;
                if (addMenuService != null)
                {
                    addMenuService.DropDownItemClicked -= value2;
                }
                _AddMenuService = value;
                addMenuService = _AddMenuService;
                if (addMenuService != null)
                {
                    addMenuService.DropDownItemClicked += value2;
                }
            }
        }

        internal virtual ToolStripMenuItem DelMenuService
        {
            [CompilerGenerated]
            get
            {
                return _DelMenuService;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ToolStripItemClickedEventHandler value2 = DelMenuService_DropDownItemClicked;
                ToolStripMenuItem delMenuService = _DelMenuService;
                if (delMenuService != null)
                {
                    delMenuService.DropDownItemClicked -= value2;
                }
                _DelMenuService = value;
                delMenuService = _DelMenuService;
                if (delMenuService != null)
                {
                    delMenuService.DropDownItemClicked += value2;
                }
            }
        }

        internal virtual ToolStripMenuItem SearchMenuService
        {
            [CompilerGenerated]
            get
            {
                return _SearchMenuService;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ToolStripItemClickedEventHandler value2 = SearchMenuService_DropDownItemClicked;
                ToolStripMenuItem searchMenuService = _SearchMenuService;
                if (searchMenuService != null)
                {
                    searchMenuService.DropDownItemClicked -= value2;
                }
                _SearchMenuService = value;
                searchMenuService = _SearchMenuService;
                if (searchMenuService != null)
                {
                    searchMenuService.DropDownItemClicked += value2;
                }
            }
        }

        internal virtual ToolStripMenuItem Group
        {
            [CompilerGenerated]
            get
            {
                return _Group;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ToolStripItemClickedEventHandler value2 = Group_DropDownItemClicked;
                ToolStripMenuItem group = _Group;
                if (group != null)
                {
                    group.DropDownItemClicked -= value2;
                }
                _Group = value;
                group = _Group;
                if (group != null)
                {
                    group.DropDownItemClicked += value2;
                }
            }
        }

        [field: AccessedThroughProperty("Customer")]
        internal virtual ToolStripMenuItem Customer
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("AliasCustomer")]
        internal virtual ToolStripMenuItem AliasCustomer
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("MenuStripFM")]
        internal virtual MenuStrip MenuStripFM
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual ContextMenuStrip CMS_ListAccesClient
        {
            [CompilerGenerated]
            get
            {
                return _CMS_ListAccesClient;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ToolStripItemClickedEventHandler value2 = CMS_ListAccesClient_ItemClicked;
                CancelEventHandler value3 = CMS_ListAccesClient_Opening;
                ContextMenuStrip cMS_ListAccesClient = _CMS_ListAccesClient;
                if (cMS_ListAccesClient != null)
                {
                    cMS_ListAccesClient.ItemClicked -= value2;
                    cMS_ListAccesClient.Opening -= value3;
                }
                _CMS_ListAccesClient = value;
                cMS_ListAccesClient = _CMS_ListAccesClient;
                if (cMS_ListAccesClient != null)
                {
                    cMS_ListAccesClient.ItemClicked += value2;
                    cMS_ListAccesClient.Opening += value3;
                }
            }
        }

        [field: AccessedThroughProperty("StatusStripFM")]
        internal virtual StatusStrip StatusStripFM
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

        internal virtual ContextMenuStrip CMS_ListPatch
        {
            [CompilerGenerated]
            get
            {
                return _CMS_ListPatch;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ToolStripItemClickedEventHandler value2 = CMS_ListPatch_ItemClicked;
                CancelEventHandler value3 = CMS_ListPatch_Opening;
                ContextMenuStrip cMS_ListPatch = _CMS_ListPatch;
                if (cMS_ListPatch != null)
                {
                    cMS_ListPatch.ItemClicked -= value2;
                    cMS_ListPatch.Opening -= value3;
                }
                _CMS_ListPatch = value;
                cMS_ListPatch = _CMS_ListPatch;
                if (cMS_ListPatch != null)
                {
                    cMS_ListPatch.ItemClicked += value2;
                    cMS_ListPatch.Opening += value3;
                }
            }
        }

        internal virtual ContextMenuStrip CMS_ListSelectPatch
        {
            [CompilerGenerated]
            get
            {
                return _CMS_ListSelectPatch;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ToolStripItemClickedEventHandler value2 = CMS_ListSelectPatch_ItemClicked;
                CancelEventHandler value3 = CMS_ListSelectPatch_Opening;
                ContextMenuStrip cMS_ListSelectPatch = _CMS_ListSelectPatch;
                if (cMS_ListSelectPatch != null)
                {
                    cMS_ListSelectPatch.ItemClicked -= value2;
                    cMS_ListSelectPatch.Opening -= value3;
                }
                _CMS_ListSelectPatch = value;
                cMS_ListSelectPatch = _CMS_ListSelectPatch;
                if (cMS_ListSelectPatch != null)
                {
                    cMS_ListSelectPatch.ItemClicked += value2;
                    cMS_ListSelectPatch.Opening += value3;
                }
            }
        }

        internal virtual ContextMenuStrip CMS_ListSelectClient
        {
            [CompilerGenerated]
            get
            {
                return _CMS_ListSelectClient;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                ToolStripItemClickedEventHandler value2 = CMS_ListSelectClient_ItemClicked;
                CancelEventHandler value3 = CMS_ListSelectClient_Opening;
                ContextMenuStrip cMS_ListSelectClient = _CMS_ListSelectClient;
                if (cMS_ListSelectClient != null)
                {
                    cMS_ListSelectClient.ItemClicked -= value2;
                    cMS_ListSelectClient.Opening -= value3;
                }
                _CMS_ListSelectClient = value;
                cMS_ListSelectClient = _CMS_ListSelectClient;
                if (cMS_ListSelectClient != null)
                {
                    cMS_ListSelectClient.ItemClicked += value2;
                    cMS_ListSelectClient.Opening += value3;
                }
            }
        }

        [field: AccessedThroughProperty("ToolStripStatusLabelFM")]
        internal virtual ToolStripStatusLabel ToolStripStatusLabelFM
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("ToolTip1")]
        internal virtual ToolTip ToolTip1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("ToolStripStatusLabelDisk")]
        internal virtual ToolStripStatusLabel ToolStripStatusLabelDisk
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("Label_version")]
        internal virtual Label Label_version
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual ListBox ListSelectClient
        {
            [CompilerGenerated]
            get
            {
                return _ListSelectClient;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                MouseEventHandler value2 = ListSelectClient_MouseDoubleClick;
                MouseEventHandler value3 = ListSelectClient_MouseDown;
                EventHandler value4 = ListSelectClient_MouseLeave;
                MouseEventHandler value5 = ListSelectClient_MouseMove;
                ListBox listSelectClient = _ListSelectClient;
                if (listSelectClient != null)
                {
                    listSelectClient.MouseDoubleClick -= value2;
                    listSelectClient.MouseDown -= value3;
                    listSelectClient.MouseLeave -= value4;
                    listSelectClient.MouseMove -= value5;
                }
                _ListSelectClient = value;
                listSelectClient = _ListSelectClient;
                if (listSelectClient != null)
                {
                    listSelectClient.MouseDoubleClick += value2;
                    listSelectClient.MouseDown += value3;
                    listSelectClient.MouseLeave += value4;
                    listSelectClient.MouseMove += value5;
                }
            }
        }

        [field: AccessedThroughProperty("Label1")]
        internal virtual Label Label1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual ListBox ListAccesClient
        {
            [CompilerGenerated]
            get
            {
                return _ListAccesClient;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                MouseEventHandler value2 = ListAccesClient_MouseDoubleClick;
                MouseEventHandler value3 = ListAccesClient_MouseDown;
                EventHandler value4 = ListAccesClient_MouseLeave;
                MouseEventHandler value5 = ListAccesClient_MouseMove;
                ListBox listAccesClient = _ListAccesClient;
                if (listAccesClient != null)
                {
                    listAccesClient.MouseDoubleClick -= value2;
                    listAccesClient.MouseDown -= value3;
                    listAccesClient.MouseLeave -= value4;
                    listAccesClient.MouseMove -= value5;
                }
                _ListAccesClient = value;
                listAccesClient = _ListAccesClient;
                if (listAccesClient != null)
                {
                    listAccesClient.MouseDoubleClick += value2;
                    listAccesClient.MouseDown += value3;
                    listAccesClient.MouseLeave += value4;
                    listAccesClient.MouseMove += value5;
                }
            }
        }

        [field: AccessedThroughProperty("Label2")]
        internal virtual Label Label2
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("Label3")]
        internal virtual Label Label3
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual ListBox ListSelectPatch
        {
            [CompilerGenerated]
            get
            {
                return _ListSelectPatch;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                MouseEventHandler value2 = ListSelectPatch_MouseDoubleClick;
                MouseEventHandler value3 = ListSelectPatch_MouseDown;
                EventHandler value4 = ListSelectPatch_MouseLeave;
                MouseEventHandler value5 = ListSelectPatch_MouseMove;
                ListBox listSelectPatch = _ListSelectPatch;
                if (listSelectPatch != null)
                {
                    listSelectPatch.MouseDoubleClick -= value2;
                    listSelectPatch.MouseDown -= value3;
                    listSelectPatch.MouseLeave -= value4;
                    listSelectPatch.MouseMove -= value5;
                }
                _ListSelectPatch = value;
                listSelectPatch = _ListSelectPatch;
                if (listSelectPatch != null)
                {
                    listSelectPatch.MouseDoubleClick += value2;
                    listSelectPatch.MouseDown += value3;
                    listSelectPatch.MouseLeave += value4;
                    listSelectPatch.MouseMove += value5;
                }
            }
        }

        [field: AccessedThroughProperty("SplitContainer1")]
        internal virtual SplitContainer SplitContainer1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual SplitContainer SplitContainer2
        {
            [CompilerGenerated]
            get
            {
                return _SplitContainer2;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                MouseEventHandler value2 = SplitContainer2_MouseDoubleClick;
                SplitContainer splitContainer = _SplitContainer2;
                if (splitContainer != null)
                {
                    splitContainer.MouseDoubleClick -= value2;
                }
                _SplitContainer2 = value;
                splitContainer = _SplitContainer2;
                if (splitContainer != null)
                {
                    splitContainer.MouseDoubleClick += value2;
                }
            }
        }

        internal virtual SplitContainer SplitContainer3
        {
            [CompilerGenerated]
            get
            {
                return _SplitContainer3;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                MouseEventHandler value2 = SplitContainer3_MouseDoubleClick;
                SplitContainer splitContainer = _SplitContainer3;
                if (splitContainer != null)
                {
                    splitContainer.MouseDoubleClick -= value2;
                }
                _SplitContainer3 = value;
                splitContainer = _SplitContainer3;
                if (splitContainer != null)
                {
                    splitContainer.MouseDoubleClick += value2;
                }
            }
        }

        internal virtual SplitContainer SplitContainer4
        {
            [CompilerGenerated]
            get
            {
                return _SplitContainer4;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                MouseEventHandler value2 = SplitContainer4_MouseDoubleClick;
                SplitContainer splitContainer = _SplitContainer4;
                if (splitContainer != null)
                {
                    splitContainer.MouseDoubleClick -= value2;
                }
                _SplitContainer4 = value;
                splitContainer = _SplitContainer4;
                if (splitContainer != null)
                {
                    splitContainer.MouseDoubleClick += value2;
                }
            }
        }

        internal virtual Button ButtonFiltreUpdates
        {
            [CompilerGenerated]
            get
            {
                return _ButtonFiltreUpdates;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = ButtonFiltreUpdates_Click;
                Button buttonFiltreUpdates = _ButtonFiltreUpdates;
                if (buttonFiltreUpdates != null)
                {
                    buttonFiltreUpdates.Click -= value2;
                }
                _ButtonFiltreUpdates = value;
                buttonFiltreUpdates = _ButtonFiltreUpdates;
                if (buttonFiltreUpdates != null)
                {
                    buttonFiltreUpdates.Click += value2;
                }
            }
        }

        internal virtual ListBox ListPatch
        {
            [CompilerGenerated]
            get
            {
                return _ListPatch;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                MouseEventHandler value2 = ListPatch_MouseDoubleClick;
                MouseEventHandler value3 = ListPatch_MouseDown;
                EventHandler value4 = ListPatch_MouseLeave;
                MouseEventHandler value5 = ListPatch_MouseMove;
                ListBox listPatch = _ListPatch;
                if (listPatch != null)
                {
                    listPatch.MouseDoubleClick -= value2;
                    listPatch.MouseDown -= value3;
                    listPatch.MouseLeave -= value4;
                    listPatch.MouseMove -= value5;
                }
                _ListPatch = value;
                listPatch = _ListPatch;
                if (listPatch != null)
                {
                    listPatch.MouseDoubleClick += value2;
                    listPatch.MouseDown += value3;
                    listPatch.MouseLeave += value4;
                    listPatch.MouseMove += value5;
                }
            }
        }

        internal virtual TextBox FiltrePatch
        {
            [CompilerGenerated]
            get
            {
                return _FiltrePatch;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                KeyEventHandler value2 = FiltrePatch_KeyDown;
                MouseEventHandler value3 = FiltrePatch_MouseDoubleClick;
                EventHandler value4 = FiltrePatch_TextChanged;
                TextBox filtrePatch = _FiltrePatch;
                if (filtrePatch != null)
                {
                    filtrePatch.KeyDown -= value2;
                    filtrePatch.MouseDoubleClick -= value3;
                    filtrePatch.TextChanged -= value4;
                }
                _FiltrePatch = value;
                filtrePatch = _FiltrePatch;
                if (filtrePatch != null)
                {
                    filtrePatch.KeyDown += value2;
                    filtrePatch.MouseDoubleClick += value3;
                    filtrePatch.TextChanged += value4;
                }
            }
        }

        [field: AccessedThroughProperty("Label5")]
        internal virtual Label Label5
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("Label4")]
        internal virtual Label Label4
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("SplitContainer5")]
        internal virtual SplitContainer SplitContainer5
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("SplitContainer7")]
        internal virtual SplitContainer SplitContainer7
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("SplitContainer8")]
        internal virtual SplitContainer SplitContainer8
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual Button F2
        {
            [CompilerGenerated]
            get
            {
                return _F2;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = F2_Click;
                Button f = _F2;
                if (f != null)
                {
                    f.Click -= value2;
                }
                _F2 = value;
                f = _F2;
                if (f != null)
                {
                    f.Click += value2;
                }
            }
        }

        internal virtual Button F3
        {
            [CompilerGenerated]
            get
            {
                return _F3;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = F3_Click;
                Button f = _F3;
                if (f != null)
                {
                    f.Click -= value2;
                }
                _F3 = value;
                f = _F3;
                if (f != null)
                {
                    f.Click += value2;
                }
            }
        }

        [field: AccessedThroughProperty("SplitContainer9")]
        internal virtual SplitContainer SplitContainer9
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual Button F4
        {
            [CompilerGenerated]
            get
            {
                return _F4;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = F4_Click;
                Button f = _F4;
                if (f != null)
                {
                    f.Click -= value2;
                }
                _F4 = value;
                f = _F4;
                if (f != null)
                {
                    f.Click += value2;
                }
            }
        }

        internal virtual Button F5
        {
            [CompilerGenerated]
            get
            {
                return _F5;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = F5_Click;
                Button f = _F5;
                if (f != null)
                {
                    f.Click -= value2;
                }
                _F5 = value;
                f = _F5;
                if (f != null)
                {
                    f.Click += value2;
                }
            }
        }

        [field: AccessedThroughProperty("SplitContainer6")]
        internal virtual SplitContainer SplitContainer6
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("SplitContainer10")]
        internal virtual SplitContainer SplitContainer10
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual Button F6
        {
            [CompilerGenerated]
            get
            {
                return _F6;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = F6_Click;
                Button f = _F6;
                if (f != null)
                {
                    f.Click -= value2;
                }
                _F6 = value;
                f = _F6;
                if (f != null)
                {
                    f.Click += value2;
                }
            }
        }

        internal virtual Button F9
        {
            [CompilerGenerated]
            get
            {
                return _F9;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = F9_Click;
                Button f = _F9;
                if (f != null)
                {
                    f.Click -= value2;
                }
                _F9 = value;
                f = _F9;
                if (f != null)
                {
                    f.Click += value2;
                }
            }
        }

        internal virtual Button F10
        {
            [CompilerGenerated]
            get
            {
                return _F10;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = F10_Click;
                Button f = _F10;
                if (f != null)
                {
                    f.Click -= value2;
                }
                _F10 = value;
                f = _F10;
                if (f != null)
                {
                    f.Click += value2;
                }
            }
        }

        [field: AccessedThroughProperty("FolderBrowserDialogZip")]
        internal virtual FolderBrowserDialog FolderBrowserDialogZip
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("OpenFilePatchList")]
        internal virtual OpenFileDialog OpenFilePatchList
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

    }
}