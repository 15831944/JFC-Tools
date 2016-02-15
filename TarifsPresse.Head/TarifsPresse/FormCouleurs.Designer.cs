namespace TarifsPresse
{
    partial class FormCouleurs
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
            this.listViewCouleurs = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.buttonMapperCouleur = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxSelectedCouleurCode = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.textBoxSelectedCouleur = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBoxCouleurToMap = new System.Windows.Forms.TextBox();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // listViewCouleurs
            // 
            this.listViewCouleurs.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1});
            this.listViewCouleurs.Location = new System.Drawing.Point(13, 37);
            this.listViewCouleurs.MultiSelect = false;
            this.listViewCouleurs.Name = "listViewCouleurs";
            this.listViewCouleurs.Size = new System.Drawing.Size(227, 270);
            this.listViewCouleurs.TabIndex = 0;
            this.listViewCouleurs.UseCompatibleStateImageBehavior = false;
            this.listViewCouleurs.View = System.Windows.Forms.View.Details;
            this.listViewCouleurs.SelectedIndexChanged += new System.EventHandler(this.listView1_SelectedIndexChanged);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Couleurs";
            this.columnHeader1.Width = 222;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(142, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Liste des couleurs connues :";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.buttonMapperCouleur);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.textBoxSelectedCouleurCode);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.textBoxSelectedCouleur);
            this.groupBox1.Location = new System.Drawing.Point(246, 79);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(227, 228);
            this.groupBox1.TabIndex = 2;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Couleur Selectionnée";
            // 
            // buttonMapperCouleur
            // 
            this.buttonMapperCouleur.Enabled = false;
            this.buttonMapperCouleur.Location = new System.Drawing.Point(30, 152);
            this.buttonMapperCouleur.Name = "buttonMapperCouleur";
            this.buttonMapperCouleur.Size = new System.Drawing.Size(166, 23);
            this.buttonMapperCouleur.TabIndex = 4;
            this.buttonMapperCouleur.Text = "Associer cette couleur";
            this.buttonMapperCouleur.UseVisualStyleBackColor = true;
            this.buttonMapperCouleur.Click += new System.EventHandler(this.buttonMapperCouleur_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(7, 59);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(160, 13);
            this.label3.TabIndex = 3;
            this.label3.Text = "Le code de la couleurs (1 char) :";
            // 
            // textBoxSelectedCouleurCode
            // 
            this.textBoxSelectedCouleurCode.Location = new System.Drawing.Point(6, 75);
            this.textBoxSelectedCouleurCode.Name = "textBoxSelectedCouleurCode";
            this.textBoxSelectedCouleurCode.ReadOnly = true;
            this.textBoxSelectedCouleurCode.Size = new System.Drawing.Size(214, 20);
            this.textBoxSelectedCouleurCode.TabIndex = 2;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(7, 20);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(117, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Le nom de la couleurs :";
            // 
            // textBoxSelectedCouleur
            // 
            this.textBoxSelectedCouleur.Location = new System.Drawing.Point(6, 36);
            this.textBoxSelectedCouleur.Name = "textBoxSelectedCouleur";
            this.textBoxSelectedCouleur.ReadOnly = true;
            this.textBoxSelectedCouleur.Size = new System.Drawing.Size(214, 20);
            this.textBoxSelectedCouleur.TabIndex = 0;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(253, 37);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(163, 13);
            this.label4.TabIndex = 6;
            this.label4.Text = "Le nom de la couleur à associer :";
            // 
            // textBoxCouleurToMap
            // 
            this.textBoxCouleurToMap.Location = new System.Drawing.Point(252, 53);
            this.textBoxCouleurToMap.Name = "textBoxCouleurToMap";
            this.textBoxCouleurToMap.ReadOnly = true;
            this.textBoxCouleurToMap.Size = new System.Drawing.Size(214, 20);
            this.textBoxCouleurToMap.TabIndex = 5;
            // 
            // FormCouleurs
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(486, 319);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.textBoxCouleurToMap);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.listViewCouleurs);
            this.Name = "FormCouleurs";
            this.Text = "Choisir la couleur correspondante ou en créer une nouvelle";
            this.Load += new System.EventHandler(this.FormCouleurs_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView listViewCouleurs;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button buttonMapperCouleur;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBoxSelectedCouleurCode;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox textBoxSelectedCouleur;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBoxCouleurToMap;
        private System.Windows.Forms.ColumnHeader columnHeader1;
    }
}