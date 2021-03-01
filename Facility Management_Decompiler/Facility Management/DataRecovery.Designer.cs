
using Facility_Management.Properties;
using System;
using System.ComponentModel;
using System.Drawing;
using System.Runtime.CompilerServices;
using System.Windows.Forms;

namespace Facility_Management
{
    partial class DataRecovery
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
            ComboBox1 = new System.Windows.Forms.ComboBox();
            Label1 = new System.Windows.Forms.Label();
            ToolStripProgressBar1 = new System.Windows.Forms.ToolStripProgressBar();
            ToolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            FolderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            SaveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            SplitContainer1 = new System.Windows.Forms.SplitContainer();
            TreeViewList = new System.Windows.Forms.TreeView();
            Label2 = new System.Windows.Forms.Label();
            Label3 = new System.Windows.Forms.Label();
            TreeViewSelected = new System.Windows.Forms.TreeView();
            StatusStrip1 = new System.Windows.Forms.StatusStrip();
            ToolStripProgressBar2 = new System.Windows.Forms.ToolStripProgressBar();
            ToolStripStatusLabel2 = new System.Windows.Forms.ToolStripStatusLabel();
            ButtonUnZip = new System.Windows.Forms.Button();
            ButtonZip = new System.Windows.Forms.Button();
            ButtonCreating = new System.Windows.Forms.Button();
            ButtonExit = new System.Windows.Forms.Button();
            ButtonParcourir = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)SplitContainer1).BeginInit();
            SplitContainer1.Panel1.SuspendLayout();
            SplitContainer1.Panel2.SuspendLayout();
            SplitContainer1.SuspendLayout();
            StatusStrip1.SuspendLayout();
            SuspendLayout();
            ComboBox1.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            ComboBox1.FormattingEnabled = true;
            ComboBox1.Location = new System.Drawing.Point(12, 25);
            ComboBox1.Name = "ComboBox1";
            ComboBox1.Size = new System.Drawing.Size(426, 21);
            ComboBox1.Sorted = true;
            ComboBox1.TabIndex = 3;
            Label1.AutoSize = true;
            Label1.Location = new System.Drawing.Point(9, 9);
            Label1.Name = "Label1";
            Label1.Size = new System.Drawing.Size(73, 13);
            Label1.TabIndex = 4;
            Label1.Text = "Source name:";
            ToolStripProgressBar1.Name = "ToolStripProgressBar1";
            ToolStripProgressBar1.Size = new System.Drawing.Size(100, 16);
            ToolStripStatusLabel1.Name = "ToolStripStatusLabel1";
            ToolStripStatusLabel1.Size = new System.Drawing.Size(0, 17);
            SplitContainer1.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            SplitContainer1.Location = new System.Drawing.Point(12, 52);
            SplitContainer1.Name = "SplitContainer1";
            SplitContainer1.Panel1.Controls.Add(TreeViewList);
            SplitContainer1.Panel1.Controls.Add(Label2);
            SplitContainer1.Panel2.Controls.Add(Label3);
            SplitContainer1.Panel2.Controls.Add(TreeViewSelected);
            SplitContainer1.Size = new System.Drawing.Size(460, 240);
            SplitContainer1.SplitterDistance = 234;
            SplitContainer1.TabIndex = 27;
            TreeViewList.AllowDrop = true;
            TreeViewList.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            TreeViewList.Location = new System.Drawing.Point(3, 16);
            TreeViewList.Name = "TreeViewList";
            TreeViewList.Size = new System.Drawing.Size(228, 221);
            TreeViewList.TabIndex = 21;
            Label2.AutoSize = true;
            Label2.Location = new System.Drawing.Point(3, 0);
            Label2.Name = "Label2";
            Label2.Size = new System.Drawing.Size(72, 13);
            Label2.TabIndex = 20;
            Label2.Text = "List of waves:";
            Label3.AutoSize = true;
            Label3.Location = new System.Drawing.Point(3, 0);
            Label3.Name = "Label3";
            Label3.Size = new System.Drawing.Size(115, 13);
            Label3.TabIndex = 26;
            Label3.Text = "List of selected waves:";
            TreeViewSelected.AllowDrop = true;
            TreeViewSelected.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            TreeViewSelected.Location = new System.Drawing.Point(3, 16);
            TreeViewSelected.Name = "TreeViewSelected";
            TreeViewSelected.Size = new System.Drawing.Size(216, 221);
            TreeViewSelected.TabIndex = 25;
            StatusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[2] { ToolStripProgressBar2, ToolStripStatusLabel2 });
            StatusStrip1.Location = new System.Drawing.Point(0, 340);
            StatusStrip1.Name = "StatusStrip1";
            StatusStrip1.Size = new System.Drawing.Size(484, 22);
            StatusStrip1.TabIndex = 28;
            StatusStrip1.Text = "StatusStrip1";
            ToolStripProgressBar2.Name = "ToolStripProgressBar2";
            ToolStripProgressBar2.Size = new System.Drawing.Size(100, 16);
            ToolStripStatusLabel2.Name = "ToolStripStatusLabel2";
            ToolStripStatusLabel2.Size = new System.Drawing.Size(120, 17);
            ToolStripStatusLabel2.Text = "ToolStripStatusLabel2";
            ButtonUnZip.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left;
            ButtonUnZip.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ButtonUnZip.Image = Resources.compress;
            ButtonUnZip.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            ButtonUnZip.Location = new System.Drawing.Point(126, 298);
            ButtonUnZip.Name = "ButtonUnZip";
            ButtonUnZip.Size = new System.Drawing.Size(108, 39);
            ButtonUnZip.TabIndex = 26;
            ButtonUnZip.Text = "UnZip   ";
            ButtonUnZip.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            ButtonUnZip.UseVisualStyleBackColor = true;
            ButtonUnZip.Visible = false;
            ButtonZip.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left;
            ButtonZip.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ButtonZip.Image = Resources.zip1;
            ButtonZip.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            ButtonZip.Location = new System.Drawing.Point(12, 298);
            ButtonZip.Name = "ButtonZip";
            ButtonZip.Size = new System.Drawing.Size(108, 39);
            ButtonZip.TabIndex = 25;
            ButtonZip.Text = "Zip      ";
            ButtonZip.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            ButtonZip.UseVisualStyleBackColor = true;
            ButtonCreating.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right;
            ButtonCreating.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ButtonCreating.Image = Resources.Update;
            ButtonCreating.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            ButtonCreating.Location = new System.Drawing.Point(250, 298);
            ButtonCreating.Name = "ButtonCreating";
            ButtonCreating.Size = new System.Drawing.Size(108, 39);
            ButtonCreating.TabIndex = 6;
            ButtonCreating.Text = "Upgrade";
            ButtonCreating.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            ButtonCreating.UseVisualStyleBackColor = true;
            ButtonExit.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right;
            ButtonExit.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
            ButtonExit.Image = Resources.sortie;
            ButtonExit.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            ButtonExit.Location = new System.Drawing.Point(364, 298);
            ButtonExit.Name = "ButtonExit";
            ButtonExit.Size = new System.Drawing.Size(108, 39);
            ButtonExit.TabIndex = 0;
            ButtonExit.Text = "Exit     ";
            ButtonExit.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
            ButtonExit.UseVisualStyleBackColor = true;
            ButtonParcourir.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right;
            ButtonParcourir.Location = new System.Drawing.Point(444, 23);
            ButtonParcourir.Name = "ButtonParcourir";
            ButtonParcourir.Size = new System.Drawing.Size(25, 23);
            ButtonParcourir.TabIndex = 29;
            ButtonParcourir.Text = "...";
            ButtonParcourir.UseVisualStyleBackColor = true;
            base.AutoScaleDimensions = new System.Drawing.SizeF(6f, 13f);
            base.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            base.ClientSize = new System.Drawing.Size(484, 362);
            base.Controls.Add(ButtonParcourir);
            base.Controls.Add(StatusStrip1);
            base.Controls.Add(SplitContainer1);
            base.Controls.Add(ButtonUnZip);
            base.Controls.Add(ButtonZip);
            base.Controls.Add(ButtonCreating);
            base.Controls.Add(Label1);
            base.Controls.Add(ComboBox1);
            base.Controls.Add(ButtonExit);
            MinimumSize = new System.Drawing.Size(500, 400);
            base.Name = "DataRecovery";
            base.ShowInTaskbar = false;
            base.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            Text = "Data Recovery";
            SplitContainer1.Panel1.ResumeLayout(false);
            SplitContainer1.Panel1.PerformLayout();
            SplitContainer1.Panel2.ResumeLayout(false);
            SplitContainer1.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)SplitContainer1).EndInit();
            SplitContainer1.ResumeLayout(false);
            StatusStrip1.ResumeLayout(false);
            StatusStrip1.PerformLayout();
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        [CompilerGenerated]
        [AccessedThroughProperty("ButtonExit")]
        private Button _ButtonExit;

        [CompilerGenerated]
        [AccessedThroughProperty("ComboBox1")]
        private ComboBox _ComboBox1;

        [CompilerGenerated]
        [AccessedThroughProperty("Label1")]
        private Label _Label1;

        [CompilerGenerated]
        [AccessedThroughProperty("ButtonCreating")]
        private Button _ButtonCreating;

        [CompilerGenerated]
        [AccessedThroughProperty("ToolStripStatusLabel1")]
        private ToolStripStatusLabel _ToolStripStatusLabel1;

        [CompilerGenerated]
        [AccessedThroughProperty("ToolStripProgressBar1")]
        private ToolStripProgressBar _ToolStripProgressBar1;

        [CompilerGenerated]
        [AccessedThroughProperty("ButtonZip")]
        private Button _ButtonZip;

        [CompilerGenerated]
        [AccessedThroughProperty("ButtonUnZip")]
        private Button _ButtonUnZip;

        [CompilerGenerated]
        [AccessedThroughProperty("FolderBrowserDialog1")]
        private FolderBrowserDialog _FolderBrowserDialog1;

        [CompilerGenerated]
        [AccessedThroughProperty("SaveFileDialog1")]
        private SaveFileDialog _SaveFileDialog1;

        [CompilerGenerated]
        [AccessedThroughProperty("SplitContainer1")]
        private SplitContainer _SplitContainer1;

        [CompilerGenerated]
        [AccessedThroughProperty("TreeViewList")]
        private TreeView _TreeViewList;

        [CompilerGenerated]
        [AccessedThroughProperty("Label2")]
        private Label _Label2;

        [CompilerGenerated]
        [AccessedThroughProperty("Label3")]
        private Label _Label3;

        [CompilerGenerated]
        [AccessedThroughProperty("TreeViewSelected")]
        private TreeView _TreeViewSelected;

        [CompilerGenerated]
        [AccessedThroughProperty("StatusStrip1")]
        private StatusStrip _StatusStrip1;

        [CompilerGenerated]
        [AccessedThroughProperty("ToolStripProgressBar2")]
        private ToolStripProgressBar _ToolStripProgressBar2;

        [CompilerGenerated]
        [AccessedThroughProperty("ToolStripStatusLabel2")]
        private ToolStripStatusLabel _ToolStripStatusLabel2;

        [CompilerGenerated]
        [AccessedThroughProperty("ButtonParcourir")]
        private Button _ButtonParcourir;

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

        internal virtual ComboBox ComboBox1
        {
            [CompilerGenerated]
            get
            {
                return _ComboBox1;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = ComboBox1_SelectedIndexChanged;
                ComboBox comboBox = _ComboBox1;
                if (comboBox != null)
                {
                    comboBox.SelectedIndexChanged -= value2;
                }
                _ComboBox1 = value;
                comboBox = _ComboBox1;
                if (comboBox != null)
                {
                    comboBox.SelectedIndexChanged += value2;
                }
            }
        }

        [field: AccessedThroughProperty("Label1")]
        internal virtual Label Label1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

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

        [field: AccessedThroughProperty("ToolStripStatusLabel1")]
        internal virtual ToolStripStatusLabel ToolStripStatusLabel1
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

        internal virtual Button ButtonZip
        {
            [CompilerGenerated]
            get
            {
                return _ButtonZip;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = ButtonZip_Click;
                Button buttonZip = _ButtonZip;
                if (buttonZip != null)
                {
                    buttonZip.Click -= value2;
                }
                _ButtonZip = value;
                buttonZip = _ButtonZip;
                if (buttonZip != null)
                {
                    buttonZip.Click += value2;
                }
            }
        }

        [field: AccessedThroughProperty("ButtonUnZip")]
        internal virtual Button ButtonUnZip
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("FolderBrowserDialog1")]
        internal virtual FolderBrowserDialog FolderBrowserDialog1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("SaveFileDialog1")]
        internal virtual SaveFileDialog SaveFileDialog1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("SplitContainer1")]
        internal virtual SplitContainer SplitContainer1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual TreeView TreeViewList
        {
            [CompilerGenerated]
            get
            {
                return _TreeViewList;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                TreeNodeMouseClickEventHandler value2 = TreeViewList_NodeMouseDoubleClick;
                ItemDragEventHandler value3 = TreeViewList_ItemDrag;
                DragEventHandler value4 = TreeViewList_DragEnter;
                DragEventHandler value5 = TreeViewList_DragDrop;
                TreeView treeViewList = _TreeViewList;
                if (treeViewList != null)
                {
                    treeViewList.NodeMouseDoubleClick -= value2;
                    treeViewList.ItemDrag -= value3;
                    treeViewList.DragEnter -= value4;
                    treeViewList.DragDrop -= value5;
                }
                _TreeViewList = value;
                treeViewList = _TreeViewList;
                if (treeViewList != null)
                {
                    treeViewList.NodeMouseDoubleClick += value2;
                    treeViewList.ItemDrag += value3;
                    treeViewList.DragEnter += value4;
                    treeViewList.DragDrop += value5;
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

        internal virtual TreeView TreeViewSelected
        {
            [CompilerGenerated]
            get
            {
                return _TreeViewSelected;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                TreeNodeMouseClickEventHandler value2 = TreeViewSelected_NodeMouseDoubleClick;
                ItemDragEventHandler value3 = TreeViewSelected_ItemDrag;
                DragEventHandler value4 = TreeViewSelected_DragEnter;
                DragEventHandler value5 = TreeViewSelected_DragDrop;
                TreeView treeViewSelected = _TreeViewSelected;
                if (treeViewSelected != null)
                {
                    treeViewSelected.NodeMouseDoubleClick -= value2;
                    treeViewSelected.ItemDrag -= value3;
                    treeViewSelected.DragEnter -= value4;
                    treeViewSelected.DragDrop -= value5;
                }
                _TreeViewSelected = value;
                treeViewSelected = _TreeViewSelected;
                if (treeViewSelected != null)
                {
                    treeViewSelected.NodeMouseDoubleClick += value2;
                    treeViewSelected.ItemDrag += value3;
                    treeViewSelected.DragEnter += value4;
                    treeViewSelected.DragDrop += value5;
                }
            }
        }

        [field: AccessedThroughProperty("StatusStrip1")]
        internal virtual StatusStrip StatusStrip1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("ToolStripProgressBar2")]
        internal virtual ToolStripProgressBar ToolStripProgressBar2
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("ToolStripStatusLabel2")]
        internal virtual ToolStripStatusLabel ToolStripStatusLabel2
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        internal virtual Button ButtonParcourir
        {
            [CompilerGenerated]
            get
            {
                return _ButtonParcourir;
            }
            [MethodImpl(MethodImplOptions.Synchronized)]
            [CompilerGenerated]
            set
            {
                EventHandler value2 = ButtonParcourir_Click;
                Button buttonParcourir = _ButtonParcourir;
                if (buttonParcourir != null)
                {
                    buttonParcourir.Click -= value2;
                }
                _ButtonParcourir = value;
                buttonParcourir = _ButtonParcourir;
                if (buttonParcourir != null)
                {
                    buttonParcourir.Click += value2;
                }
            }
        }

    }
}