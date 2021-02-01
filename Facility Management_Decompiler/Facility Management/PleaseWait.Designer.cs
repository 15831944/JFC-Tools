
using System.Drawing;
using System.Runtime.CompilerServices;
using System.Windows.Forms;

namespace Facility_Management
{
    partial class PleaseWait
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
        private void InitializeComponent()
        {
			this.Label1 = new Label();
			this.ProgressBarPleaseWait = new ProgressBar();
			base.SuspendLayout();
			this.Label1.AutoSize = true;
			this.Label1.Font = new Font("Tahoma", 15.75f, FontStyle.Bold);
			this.Label1.Location = new Point(9, 9);
			this.Label1.Name = "Label1";
			this.Label1.Size = new Size(330, 25);
			this.Label1.TabIndex = 0;
			this.Label1.Text = "Please wait while initializing...";
			this.ProgressBarPleaseWait.Anchor = (AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right);
			this.ProgressBarPleaseWait.Location = new Point(12, 45);
			this.ProgressBarPleaseWait.Name = "ProgressBarPleaseWait";
			this.ProgressBarPleaseWait.Size = new Size(456, 23);
			this.ProgressBarPleaseWait.Style = ProgressBarStyle.Marquee;
			this.ProgressBarPleaseWait.TabIndex = 1;
			base.AutoScaleDimensions = new SizeF(6f, 13f);
			base.AutoScaleMode = AutoScaleMode.Font;
			base.ClientSize = new Size(480, 80);
			base.Controls.Add(this.ProgressBarPleaseWait);
			base.Controls.Add(this.Label1);
			base.FormBorderStyle = FormBorderStyle.None;
			base.MaximizeBox = false;
			base.MinimizeBox = false;
			base.Name = "PleaseWait";
			base.ShowIcon = false;
			base.ShowInTaskbar = false;
			base.StartPosition = FormStartPosition.CenterScreen;
			this.Text = "AutoUpdate Services - Updating";
			base.ResumeLayout(false);
			base.PerformLayout();
		}

		private Label _Label1;
		[AccessedThroughProperty("ProgressBarPleaseWait"), CompilerGenerated]
		private ProgressBar _ProgressBarPleaseWait;
		internal virtual Label Label1
		{
			get;
			[MethodImpl(MethodImplOptions.Synchronized)]
			set;
		}
		internal virtual ProgressBar ProgressBarPleaseWait
		{
			get;
			[MethodImpl(MethodImplOptions.Synchronized)]
			set;
		}

		#endregion
	}
}