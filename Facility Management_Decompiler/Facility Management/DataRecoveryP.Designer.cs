
using Facility_Management.Properties;
using System;
using System.ComponentModel;
using System.Drawing;
using System.Runtime.CompilerServices;
using System.Windows.Forms;

namespace Facility_Management
{
    partial class DataRecoveryP
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
			SplitContainer1 = new System.Windows.Forms.SplitContainer();
			TreeViewList = new System.Windows.Forms.TreeView();
			TreeViewSelected = new System.Windows.Forms.TreeView();
			StatusStrip1 = new System.Windows.Forms.StatusStrip();
			ToolStripProgressBar1 = new System.Windows.Forms.ToolStripProgressBar();
			ToolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
			ButtonCreating = new System.Windows.Forms.Button();
			ButtonExit = new System.Windows.Forms.Button();
			Label1 = new System.Windows.Forms.Label();
			ComboBox1 = new System.Windows.Forms.ComboBox();
			SplitContainer1.Panel1.SuspendLayout();
			SplitContainer1.Panel2.SuspendLayout();
			SplitContainer1.SuspendLayout();
			StatusStrip1.SuspendLayout();
			SuspendLayout();
			SplitContainer1.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
			SplitContainer1.Location = new System.Drawing.Point(12, 39);
			SplitContainer1.Name = "SplitContainer1";
			SplitContainer1.Panel1.Controls.Add(TreeViewList);
			SplitContainer1.Panel2.Controls.Add(TreeViewSelected);
			SplitContainer1.Size = new System.Drawing.Size(460, 253);
			SplitContainer1.SplitterDistance = 234;
			SplitContainer1.TabIndex = 0;
			TreeViewList.AllowDrop = true;
			TreeViewList.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
			TreeViewList.Location = new System.Drawing.Point(3, 3);
			TreeViewList.Name = "TreeViewList";
			TreeViewList.Size = new System.Drawing.Size(228, 247);
			TreeViewList.TabIndex = 22;
			TreeViewSelected.AllowDrop = true;
			TreeViewSelected.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
			TreeViewSelected.Location = new System.Drawing.Point(3, 3);
			TreeViewSelected.Name = "TreeViewSelected";
			TreeViewSelected.Size = new System.Drawing.Size(216, 247);
			TreeViewSelected.TabIndex = 26;
			StatusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[2] { ToolStripProgressBar1, ToolStripStatusLabel1 });
			StatusStrip1.Location = new System.Drawing.Point(0, 340);
			StatusStrip1.Name = "StatusStrip1";
			StatusStrip1.Size = new System.Drawing.Size(484, 22);
			StatusStrip1.TabIndex = 1;
			StatusStrip1.Text = "StatusStrip1";
			ToolStripProgressBar1.Name = "ToolStripProgressBar1";
			ToolStripProgressBar1.Size = new System.Drawing.Size(100, 16);
			ToolStripStatusLabel1.Name = "ToolStripStatusLabel1";
			ToolStripStatusLabel1.Size = new System.Drawing.Size(121, 17);
			ToolStripStatusLabel1.Text = "ToolStripStatusLabel1";
			ButtonCreating.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right;
			ButtonCreating.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25f, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, 0);
			ButtonCreating.Image = Resources.Update;
			ButtonCreating.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
			ButtonCreating.Location = new System.Drawing.Point(250, 298);
			ButtonCreating.Name = "ButtonCreating";
			ButtonCreating.Size = new System.Drawing.Size(108, 39);
			ButtonCreating.TabIndex = 8;
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
			ButtonExit.TabIndex = 7;
			ButtonExit.Text = "Exit     ";
			ButtonExit.TextAlign = System.Drawing.ContentAlignment.MiddleRight;
			ButtonExit.UseVisualStyleBackColor = true;
			Label1.AutoSize = true;
			Label1.Location = new System.Drawing.Point(12, 9);
			Label1.Name = "Label1";
			Label1.Size = new System.Drawing.Size(67, 13);
			Label1.TabIndex = 10;
			Label1.Text = "DroitsAcces:";
			ComboBox1.Anchor = System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
			ComboBox1.FormattingEnabled = true;
			ComboBox1.Location = new System.Drawing.Point(85, 12);
			ComboBox1.Name = "ComboBox1";
			ComboBox1.Size = new System.Drawing.Size(387, 21);
			ComboBox1.Sorted = true;
			ComboBox1.TabIndex = 9;
			base.AutoScaleDimensions = new System.Drawing.SizeF(6f, 13f);
			base.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			base.ClientSize = new System.Drawing.Size(484, 362);
			base.Controls.Add(Label1);
			base.Controls.Add(ComboBox1);
			base.Controls.Add(ButtonCreating);
			base.Controls.Add(ButtonExit);
			base.Controls.Add(StatusStrip1);
			base.Controls.Add(SplitContainer1);
			//base.Icon = (System.Drawing.Icon)resources.GetObject("$this.Icon");
			MinimumSize = new System.Drawing.Size(500, 400);
			base.Name = "DataRecoveryP";
			base.ShowInTaskbar = false;
			base.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			Text = "Data Recovery (BETA)";
			SplitContainer1.Panel1.ResumeLayout(false);
			SplitContainer1.Panel2.ResumeLayout(false);
			SplitContainer1.ResumeLayout(false);
			StatusStrip1.ResumeLayout(false);
			StatusStrip1.PerformLayout();
			ResumeLayout(false);
			PerformLayout();
		}

		#endregion


		[CompilerGenerated]
		[AccessedThroughProperty("SplitContainer1")]
		private SplitContainer _SplitContainer1;

		[CompilerGenerated]
		[AccessedThroughProperty("StatusStrip1")]
		private StatusStrip _StatusStrip1;

		[CompilerGenerated]
		[AccessedThroughProperty("ToolStripProgressBar1")]
		private ToolStripProgressBar _ToolStripProgressBar1;

		[CompilerGenerated]
		[AccessedThroughProperty("ToolStripStatusLabel1")]
		private ToolStripStatusLabel _ToolStripStatusLabel1;

		[CompilerGenerated]
		[AccessedThroughProperty("TreeViewList")]
		private TreeView _TreeViewList;

		[CompilerGenerated]
		[AccessedThroughProperty("TreeViewSelected")]
		private TreeView _TreeViewSelected;

		[CompilerGenerated]
		[AccessedThroughProperty("ButtonCreating")]
		private Button _ButtonCreating;

		[CompilerGenerated]
		[AccessedThroughProperty("ButtonExit")]
		private Button _ButtonExit;

		[CompilerGenerated]
		[AccessedThroughProperty("Label1")]
		private Label _Label1;

		[CompilerGenerated]
		[AccessedThroughProperty("ComboBox1")]
		private ComboBox _ComboBox1;

		[field: AccessedThroughProperty("SplitContainer1")]
		internal virtual SplitContainer SplitContainer1
		{
			get; [MethodImpl(MethodImplOptions.Synchronized)]
			set;
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

		[field: AccessedThroughProperty("Label1")]
		internal virtual Label Label1
		{
			get; [MethodImpl(MethodImplOptions.Synchronized)]
			set;
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

	}
}