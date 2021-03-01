// MultiUserEDI.PleaseWait
using Microsoft.VisualBasic.CompilerServices;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.CompilerServices;
using System.Windows.Forms;

namespace MultiUserEDI
{
    [DesignerGenerated]
    public class PleaseWait : Form
    {
        private IContainer components;

        [CompilerGenerated]
        [AccessedThroughProperty("Label1")]
        private Label _Label1;

        [CompilerGenerated]
        [AccessedThroughProperty("ProgressBarPleaseWait")]
        private ProgressBar _ProgressBarPleaseWait;

        [field: AccessedThroughProperty("Label1")]
        internal virtual Label Label1
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        [field: AccessedThroughProperty("ProgressBarPleaseWait")]
        internal virtual ProgressBar ProgressBarPleaseWait
        {
            get; [MethodImpl(MethodImplOptions.Synchronized)]
            set;
        }

        public PleaseWait()
        {
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
            Label1 = new System.Windows.Forms.Label();
            ProgressBarPleaseWait = new System.Windows.Forms.ProgressBar();
            SuspendLayout();
            Label1.AutoSize = true;
            Label1.Font = new System.Drawing.Font("Tahoma", 15.75f, System.Drawing.FontStyle.Bold);
            Label1.Location = new System.Drawing.Point(9, 9);
            Label1.Name = "Label1";
            Label1.Size = new System.Drawing.Size(330, 25);
            Label1.TabIndex = 0;
            Label1.Text = "Please wait while initializing...";
            ProgressBarPleaseWait.Anchor = System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right;
            ProgressBarPleaseWait.Location = new System.Drawing.Point(12, 43);
            ProgressBarPleaseWait.Name = "ProgressBarPleaseWait";
            ProgressBarPleaseWait.Size = new System.Drawing.Size(470, 23);
            ProgressBarPleaseWait.Style = System.Windows.Forms.ProgressBarStyle.Marquee;
            ProgressBarPleaseWait.TabIndex = 1;
            base.AutoScaleDimensions = new System.Drawing.SizeF(6f, 13f);
            base.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            base.ClientSize = new System.Drawing.Size(494, 78);
            base.Controls.Add(ProgressBarPleaseWait);
            base.Controls.Add(Label1);
            base.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            base.MaximizeBox = false;
            base.MinimizeBox = false;
            base.Name = "PleaseWait";
            base.ShowIcon = false;
            base.ShowInTaskbar = false;
            base.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            Text = "AutoUpdate Services - Updating";
            ResumeLayout(false);
            PerformLayout();
        }
    }
}