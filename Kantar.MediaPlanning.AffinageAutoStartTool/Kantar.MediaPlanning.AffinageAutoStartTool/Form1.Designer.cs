
namespace Kantar.MediaPlanning.AffinageAutoStartTool
{
    partial class Form1
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            this.labelStatus = new System.Windows.Forms.Label();
            this.labelUser = new System.Windows.Forms.Label();
            this.labelDate = new System.Windows.Forms.Label();
            this.buttonRefresh = new System.Windows.Forms.Button();
            this.buttonStart = new System.Windows.Forms.Button();
            this.buttonExit = new System.Windows.Forms.Button();
            this.labelStatusValue = new System.Windows.Forms.Label();
            this.labelValueUser = new System.Windows.Forms.Label();
            this.labelValueDate = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // labelStatus
            // 
            this.labelStatus.AutoSize = true;
            this.labelStatus.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelStatus.Location = new System.Drawing.Point(12, 9);
            this.labelStatus.Name = "labelStatus";
            this.labelStatus.Size = new System.Drawing.Size(48, 16);
            this.labelStatus.TabIndex = 0;
            this.labelStatus.Text = "Status:";
            // 
            // labelUser
            // 
            this.labelUser.AutoSize = true;
            this.labelUser.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelUser.Location = new System.Drawing.Point(12, 39);
            this.labelUser.Name = "labelUser";
            this.labelUser.Size = new System.Drawing.Size(70, 16);
            this.labelUser.TabIndex = 1;
            this.labelUser.Text = "Utilisateur:";
            // 
            // labelDate
            // 
            this.labelDate.AutoSize = true;
            this.labelDate.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelDate.Location = new System.Drawing.Point(12, 67);
            this.labelDate.Name = "labelDate";
            this.labelDate.Size = new System.Drawing.Size(40, 16);
            this.labelDate.TabIndex = 2;
            this.labelDate.Text = "Date:";
            // 
            // buttonRefresh
            // 
            this.buttonRefresh.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonRefresh.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonRefresh.Location = new System.Drawing.Point(15, 95);
            this.buttonRefresh.Name = "buttonRefresh";
            this.buttonRefresh.Size = new System.Drawing.Size(75, 23);
            this.buttonRefresh.TabIndex = 3;
            this.buttonRefresh.Text = "Rafraichir";
            this.buttonRefresh.UseVisualStyleBackColor = true;
            this.buttonRefresh.Click += new System.EventHandler(this.buttonRefresh_Click);
            // 
            // buttonStart
            // 
            this.buttonStart.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonStart.Location = new System.Drawing.Point(101, 95);
            this.buttonStart.Name = "buttonStart";
            this.buttonStart.Size = new System.Drawing.Size(75, 23);
            this.buttonStart.TabIndex = 4;
            this.buttonStart.Text = "Démarrer";
            this.buttonStart.UseVisualStyleBackColor = true;
            this.buttonStart.Click += new System.EventHandler(this.buttonStart_Click);
            // 
            // buttonExit
            // 
            this.buttonExit.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.buttonExit.Location = new System.Drawing.Point(189, 95);
            this.buttonExit.Name = "buttonExit";
            this.buttonExit.Size = new System.Drawing.Size(75, 23);
            this.buttonExit.TabIndex = 5;
            this.buttonExit.Text = "Quitter";
            this.buttonExit.UseVisualStyleBackColor = true;
            this.buttonExit.Click += new System.EventHandler(this.buttonExit_Click);
            // 
            // labelStatusValue
            // 
            this.labelStatusValue.AutoSize = true;
            this.labelStatusValue.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelStatusValue.Location = new System.Drawing.Point(58, 9);
            this.labelStatusValue.Name = "labelStatusValue";
            this.labelStatusValue.Size = new System.Drawing.Size(44, 16);
            this.labelStatusValue.TabIndex = 6;
            this.labelStatusValue.Text = "Arrêté";
            // 
            // labelValueUser
            // 
            this.labelValueUser.AutoSize = true;
            this.labelValueUser.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelValueUser.Location = new System.Drawing.Point(81, 39);
            this.labelValueUser.Name = "labelValueUser";
            this.labelValueUser.Size = new System.Drawing.Size(44, 16);
            this.labelValueUser.TabIndex = 7;
            this.labelValueUser.Text = "Néant";
            // 
            // labelValueDate
            // 
            this.labelValueDate.AutoSize = true;
            this.labelValueDate.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.labelValueDate.Location = new System.Drawing.Point(50, 67);
            this.labelValueDate.Name = "labelValueDate";
            this.labelValueDate.Size = new System.Drawing.Size(54, 16);
            this.labelValueDate.TabIndex = 8;
            this.labelValueDate.Text = "Inconnu";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 121);
            this.Controls.Add(this.labelValueDate);
            this.Controls.Add(this.labelValueUser);
            this.Controls.Add(this.labelStatusValue);
            this.Controls.Add(this.buttonExit);
            this.Controls.Add(this.buttonStart);
            this.Controls.Add(this.buttonRefresh);
            this.Controls.Add(this.labelDate);
            this.Controls.Add(this.labelUser);
            this.Controls.Add(this.labelStatus);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "Form1";
            this.Text = "Status Affinage Auto";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelStatus;
        private System.Windows.Forms.Label labelUser;
        private System.Windows.Forms.Label labelDate;
        private System.Windows.Forms.Button buttonRefresh;
        private System.Windows.Forms.Button buttonStart;
        private System.Windows.Forms.Button buttonExit;
        private System.Windows.Forms.Label labelStatusValue;
        private System.Windows.Forms.Label labelValueUser;
        private System.Windows.Forms.Label labelValueDate;
    }
}

