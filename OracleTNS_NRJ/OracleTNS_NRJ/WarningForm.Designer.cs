namespace AtOracle_NRJ
{
    partial class WarningForm
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
            this.labelError = new System.Windows.Forms.Label();
            this.buttonOK = new System.Windows.Forms.Button();
            this.txtConnexion = new System.Windows.Forms.TextBox();
            this.txtArgs = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // labelError
            // 
            this.labelError.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.labelError.Location = new System.Drawing.Point(12, 23);
            this.labelError.Name = "labelError";
            this.labelError.Size = new System.Drawing.Size(359, 26);
            this.labelError.TabIndex = 0;
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(149, 196);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 1;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // txtConnexion
            // 
            this.txtConnexion.Location = new System.Drawing.Point(15, 52);
            this.txtConnexion.Multiline = true;
            this.txtConnexion.Name = "txtConnexion";
            this.txtConnexion.Size = new System.Drawing.Size(356, 71);
            this.txtConnexion.TabIndex = 2;
            // 
            // txtArgs
            // 
            this.txtArgs.Location = new System.Drawing.Point(15, 153);
            this.txtArgs.Multiline = true;
            this.txtArgs.Name = "txtArgs";
            this.txtArgs.Size = new System.Drawing.Size(356, 20);
            this.txtArgs.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(15, 130);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(107, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "Liste des arguments :";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(18, 23);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(113, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Chaine de connexion :";
            // 
            // WarningForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(383, 231);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.txtArgs);
            this.Controls.Add(this.txtConnexion);
            this.Controls.Add(this.buttonOK);
            this.Controls.Add(this.labelError);
            this.Name = "WarningForm";
            this.Text = "Connexion à la base oracle";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelError;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.TextBox txtConnexion;
        private System.Windows.Forms.TextBox txtArgs;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
    }
}