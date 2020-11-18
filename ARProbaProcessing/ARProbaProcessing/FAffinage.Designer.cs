namespace ARProbaProcessing
{
    partial class FAffinage
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
            this.cbo_panel = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.nudYear = new System.Windows.Forms.NumericUpDown();
            this.lblYear = new System.Windows.Forms.Label();
            this.btnCancel = new System.Windows.Forms.Button();
            this.btnOk = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.nudYear)).BeginInit();
            this.SuspendLayout();
            // 
            // cbo_panel
            // 
            this.cbo_panel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbo_panel.FormattingEnabled = true;
            this.cbo_panel.Items.AddRange(new object[] {
            "National",
            "Cadre",
            "Ile de France"});
            this.cbo_panel.Location = new System.Drawing.Point(102, 26);
            this.cbo_panel.Name = "cbo_panel";
            this.cbo_panel.Size = new System.Drawing.Size(440, 28);
            this.cbo_panel.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(33, 29);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(49, 20);
            this.label1.TabIndex = 1;
            this.label1.Text = "Panel";
            // 
            // nudYear
            // 
            this.nudYear.Location = new System.Drawing.Point(102, 78);
            this.nudYear.Maximum = new decimal(new int[] {
            2030,
            0,
            0,
            0});
            this.nudYear.Minimum = new decimal(new int[] {
            2018,
            0,
            0,
            0});
            this.nudYear.Name = "nudYear";
            this.nudYear.Size = new System.Drawing.Size(120, 26);
            this.nudYear.TabIndex = 2;
            this.nudYear.Value = new decimal(new int[] {
            2018,
            0,
            0,
            0});
            // 
            // lblYear
            // 
            this.lblYear.AutoSize = true;
            this.lblYear.Location = new System.Drawing.Point(31, 80);
            this.lblYear.Name = "lblYear";
            this.lblYear.Size = new System.Drawing.Size(56, 20);
            this.lblYear.TabIndex = 3;
            this.lblYear.Text = "Année";
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(303, 128);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(123, 36);
            this.btnCancel.TabIndex = 4;
            this.btnCancel.Text = "Annuler";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // btnOk
            // 
            this.btnOk.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOk.Location = new System.Drawing.Point(155, 128);
            this.btnOk.Name = "btnOk";
            this.btnOk.Size = new System.Drawing.Size(123, 36);
            this.btnOk.TabIndex = 5;
            this.btnOk.Text = "OK";
            this.btnOk.UseVisualStyleBackColor = true;
            // 
            // FAffinage
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(578, 185);
            this.Controls.Add(this.btnOk);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.lblYear);
            this.Controls.Add(this.nudYear);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cbo_panel);
            this.Name = "FAffinage";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.nudYear)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox cbo_panel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown nudYear;
        private System.Windows.Forms.Label lblYear;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.Button btnOk;
    }
}