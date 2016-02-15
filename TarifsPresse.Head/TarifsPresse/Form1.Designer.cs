namespace TarifsPresse
{
    partial class Form1
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
			this.groupBoxEtape1 = new System.Windows.Forms.GroupBox();
			this.labelEtape1 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.buttonPathParutions = new System.Windows.Forms.Button();
			this.pathParutions = new System.Windows.Forms.TextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.buttonPathTarifs = new System.Windows.Forms.Button();
			this.pathTarifs = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.buttonPathVersions = new System.Windows.Forms.Button();
			this.pathVersions = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.buttonPathSupports = new System.Windows.Forms.Button();
			this.pathSupports = new System.Windows.Forms.TextBox();
			this.progressBarSource = new System.Windows.Forms.ProgressBar();
			this.groupBoxEtape2 = new System.Windows.Forms.GroupBox();
			this.label5 = new System.Windows.Forms.Label();
			this.buttonPathCodipress = new System.Windows.Forms.Button();
			this.pathCodipress = new System.Windows.Forms.TextBox();
			this.buttonMapSupports = new System.Windows.Forms.Button();
			this.labelSupportsNotFound = new System.Windows.Forms.Label();
			this.buttonMajBases = new System.Windows.Forms.Button();
			this.groupBoxEtape3 = new System.Windows.Forms.GroupBox();
			this.buttonMapColorCode = new System.Windows.Forms.Button();
			this.labelColorCodeNotFound = new System.Windows.Forms.Label();
			this.buttonMapFormat = new System.Windows.Forms.Button();
			this.labelFormatNotFound = new System.Windows.Forms.Label();
			this.groupBox1 = new System.Windows.Forms.GroupBox();
			this.buttonImporterBaseCodipress = new System.Windows.Forms.Button();
			this.checkBoxIgnoreDiffusionNotNATIONAL = new System.Windows.Forms.CheckBox();
			this.checkBoxIgnoreTypePubliciteNotCOMMERCIALE = new System.Windows.Forms.CheckBox();
			this.checkBoxIgnoreRubriques = new System.Windows.Forms.CheckBox();
			this.labelStatusImportCodipress = new System.Windows.Forms.Label();
			this.groupBoxEtape1.SuspendLayout();
			this.groupBoxEtape2.SuspendLayout();
			this.groupBoxEtape3.SuspendLayout();
			this.groupBox1.SuspendLayout();
			this.SuspendLayout();
			// 
			// groupBoxEtape1
			// 
			this.groupBoxEtape1.Controls.Add(this.labelEtape1);
			this.groupBoxEtape1.Controls.Add(this.label4);
			this.groupBoxEtape1.Controls.Add(this.buttonPathParutions);
			this.groupBoxEtape1.Controls.Add(this.pathParutions);
			this.groupBoxEtape1.Controls.Add(this.label3);
			this.groupBoxEtape1.Controls.Add(this.buttonPathTarifs);
			this.groupBoxEtape1.Controls.Add(this.pathTarifs);
			this.groupBoxEtape1.Controls.Add(this.label2);
			this.groupBoxEtape1.Controls.Add(this.buttonPathVersions);
			this.groupBoxEtape1.Controls.Add(this.pathVersions);
			this.groupBoxEtape1.Controls.Add(this.label1);
			this.groupBoxEtape1.Controls.Add(this.buttonPathSupports);
			this.groupBoxEtape1.Controls.Add(this.pathSupports);
			this.groupBoxEtape1.Location = new System.Drawing.Point(12, 12);
			this.groupBoxEtape1.Name = "groupBoxEtape1";
			this.groupBoxEtape1.Size = new System.Drawing.Size(306, 230);
			this.groupBoxEtape1.TabIndex = 0;
			this.groupBoxEtape1.TabStop = false;
			this.groupBoxEtape1.Text = "étape 1 - bases de données source";
			// 
			// labelEtape1
			// 
			this.labelEtape1.AutoSize = true;
			this.labelEtape1.Location = new System.Drawing.Point(7, 215);
			this.labelEtape1.Name = "labelEtape1";
			this.labelEtape1.Size = new System.Drawing.Size(201, 13);
			this.labelEtape1.TabIndex = 12;
			this.labelEtape1.Text = "-> Veuillez charger les fichiers manquants";
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(7, 169);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(175, 13);
			this.label4.TabIndex = 11;
			this.label4.Text = "Fichier des parutions (PPxxxxxx.dat)";
			// 
			// buttonPathParutions
			// 
			this.buttonPathParutions.Location = new System.Drawing.Point(270, 188);
			this.buttonPathParutions.Name = "buttonPathParutions";
			this.buttonPathParutions.Size = new System.Drawing.Size(29, 19);
			this.buttonPathParutions.TabIndex = 10;
			this.buttonPathParutions.Text = "...";
			this.buttonPathParutions.UseVisualStyleBackColor = true;
			this.buttonPathParutions.Click += new System.EventHandler(this.buttonPathParutions_Click);
			// 
			// pathParutions
			// 
			this.pathParutions.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
			this.pathParutions.Location = new System.Drawing.Point(7, 188);
			this.pathParutions.Name = "pathParutions";
			this.pathParutions.ReadOnly = true;
			this.pathParutions.Size = new System.Drawing.Size(258, 20);
			this.pathParutions.TabIndex = 9;
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(7, 117);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(149, 13);
			this.label3.TabIndex = 8;
			this.label3.Text = "Fichier des tarifs (PTxxxxxx.lib)";
			// 
			// buttonPathTarifs
			// 
			this.buttonPathTarifs.Location = new System.Drawing.Point(270, 136);
			this.buttonPathTarifs.Name = "buttonPathTarifs";
			this.buttonPathTarifs.Size = new System.Drawing.Size(29, 19);
			this.buttonPathTarifs.TabIndex = 7;
			this.buttonPathTarifs.Text = "...";
			this.buttonPathTarifs.UseVisualStyleBackColor = true;
			this.buttonPathTarifs.Click += new System.EventHandler(this.buttonPathTarifs_Click);
			// 
			// pathTarifs
			// 
			this.pathTarifs.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
			this.pathTarifs.Location = new System.Drawing.Point(7, 136);
			this.pathTarifs.Name = "pathTarifs";
			this.pathTarifs.ReadOnly = true;
			this.pathTarifs.Size = new System.Drawing.Size(258, 20);
			this.pathTarifs.TabIndex = 6;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(7, 66);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(211, 13);
			this.label2.TabIndex = 5;
			this.label2.Text = "Fichier des versions de tarifs (PVxxxxxx.dat)";
			// 
			// buttonPathVersions
			// 
			this.buttonPathVersions.Location = new System.Drawing.Point(270, 85);
			this.buttonPathVersions.Name = "buttonPathVersions";
			this.buttonPathVersions.Size = new System.Drawing.Size(29, 19);
			this.buttonPathVersions.TabIndex = 4;
			this.buttonPathVersions.Text = "...";
			this.buttonPathVersions.UseVisualStyleBackColor = true;
			this.buttonPathVersions.Click += new System.EventHandler(this.buttonPathVersions_Click);
			// 
			// pathVersions
			// 
			this.pathVersions.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
			this.pathVersions.Location = new System.Drawing.Point(7, 85);
			this.pathVersions.Name = "pathVersions";
			this.pathVersions.ReadOnly = true;
			this.pathVersions.Size = new System.Drawing.Size(258, 20);
			this.pathVersions.TabIndex = 3;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(6, 20);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(167, 13);
			this.label1.TabIndex = 2;
			this.label1.Text = "Fichier des supports (PSxxxxxx.lib)";
			// 
			// buttonPathSupports
			// 
			this.buttonPathSupports.Location = new System.Drawing.Point(270, 39);
			this.buttonPathSupports.Name = "buttonPathSupports";
			this.buttonPathSupports.Size = new System.Drawing.Size(29, 19);
			this.buttonPathSupports.TabIndex = 1;
			this.buttonPathSupports.Text = "...";
			this.buttonPathSupports.UseVisualStyleBackColor = true;
			this.buttonPathSupports.Click += new System.EventHandler(this.buttonPathSupports_Click);
			// 
			// pathSupports
			// 
			this.pathSupports.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
			this.pathSupports.Location = new System.Drawing.Point(7, 39);
			this.pathSupports.Name = "pathSupports";
			this.pathSupports.ReadOnly = true;
			this.pathSupports.Size = new System.Drawing.Size(258, 20);
			this.pathSupports.TabIndex = 0;
			// 
			// progressBarSource
			// 
			this.progressBarSource.Location = new System.Drawing.Point(12, 380);
			this.progressBarSource.Name = "progressBarSource";
			this.progressBarSource.Size = new System.Drawing.Size(258, 18);
			this.progressBarSource.TabIndex = 2;
			this.progressBarSource.Visible = false;
			// 
			// groupBoxEtape2
			// 
			this.groupBoxEtape2.Controls.Add(this.labelStatusImportCodipress);
			this.groupBoxEtape2.Controls.Add(this.label5);
			this.groupBoxEtape2.Controls.Add(this.buttonPathCodipress);
			this.groupBoxEtape2.Controls.Add(this.pathCodipress);
			this.groupBoxEtape2.Enabled = false;
			this.groupBoxEtape2.Location = new System.Drawing.Point(324, 12);
			this.groupBoxEtape2.Name = "groupBoxEtape2";
			this.groupBoxEtape2.Size = new System.Drawing.Size(306, 79);
			this.groupBoxEtape2.TabIndex = 1;
			this.groupBoxEtape2.TabStop = false;
			this.groupBoxEtape2.Text = "étape 2 - fichiers codipress";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(6, 20);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(273, 13);
			this.label5.TabIndex = 5;
			this.label5.Text = "Répertoire contenant les fichiers log codipress à importer";
			// 
			// buttonPathCodipress
			// 
			this.buttonPathCodipress.Location = new System.Drawing.Point(270, 39);
			this.buttonPathCodipress.Name = "buttonPathCodipress";
			this.buttonPathCodipress.Size = new System.Drawing.Size(29, 19);
			this.buttonPathCodipress.TabIndex = 4;
			this.buttonPathCodipress.Text = "...";
			this.buttonPathCodipress.UseVisualStyleBackColor = true;
			this.buttonPathCodipress.Click += new System.EventHandler(this.buttonPathCodipress_Click);
			// 
			// pathCodipress
			// 
			this.pathCodipress.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(128)))), ((int)(((byte)(128)))));
			this.pathCodipress.Location = new System.Drawing.Point(7, 39);
			this.pathCodipress.Name = "pathCodipress";
			this.pathCodipress.ReadOnly = true;
			this.pathCodipress.Size = new System.Drawing.Size(258, 20);
			this.pathCodipress.TabIndex = 3;
			// 
			// buttonMapSupports
			// 
			this.buttonMapSupports.Enabled = false;
			this.buttonMapSupports.Location = new System.Drawing.Point(225, 19);
			this.buttonMapSupports.Name = "buttonMapSupports";
			this.buttonMapSupports.Size = new System.Drawing.Size(74, 23);
			this.buttonMapSupports.TabIndex = 14;
			this.buttonMapSupports.Text = "Associer";
			this.buttonMapSupports.UseVisualStyleBackColor = true;
			this.buttonMapSupports.Click += new System.EventHandler(this.buttonMapSupports_Click);
			// 
			// labelSupportsNotFound
			// 
			this.labelSupportsNotFound.AutoSize = true;
			this.labelSupportsNotFound.Location = new System.Drawing.Point(8, 24);
			this.labelSupportsNotFound.Name = "labelSupportsNotFound";
			this.labelSupportsNotFound.Size = new System.Drawing.Size(105, 13);
			this.labelSupportsNotFound.TabIndex = 13;
			this.labelSupportsNotFound.Text = "0 support non trouvé";
			// 
			// buttonMajBases
			// 
			this.buttonMajBases.Location = new System.Drawing.Point(225, 116);
			this.buttonMajBases.Name = "buttonMajBases";
			this.buttonMajBases.Size = new System.Drawing.Size(75, 23);
			this.buttonMajBases.TabIndex = 7;
			this.buttonMajBases.Text = "Mise à jour des bases";
			this.buttonMajBases.UseVisualStyleBackColor = true;
			this.buttonMajBases.Click += new System.EventHandler(this.buttonMajBases_Click);
			// 
			// groupBoxEtape3
			// 
			this.groupBoxEtape3.Controls.Add(this.buttonMapColorCode);
			this.groupBoxEtape3.Controls.Add(this.labelColorCodeNotFound);
			this.groupBoxEtape3.Controls.Add(this.buttonMapFormat);
			this.groupBoxEtape3.Controls.Add(this.labelFormatNotFound);
			this.groupBoxEtape3.Controls.Add(this.buttonMajBases);
			this.groupBoxEtape3.Controls.Add(this.buttonMapSupports);
			this.groupBoxEtape3.Controls.Add(this.labelSupportsNotFound);
			this.groupBoxEtape3.Enabled = false;
			this.groupBoxEtape3.Location = new System.Drawing.Point(324, 94);
			this.groupBoxEtape3.Name = "groupBoxEtape3";
			this.groupBoxEtape3.Size = new System.Drawing.Size(306, 148);
			this.groupBoxEtape3.TabIndex = 4;
			this.groupBoxEtape3.TabStop = false;
			this.groupBoxEtape3.Text = "étape 3 - association des éléments";
			// 
			// buttonMapColorCode
			// 
			this.buttonMapColorCode.Enabled = false;
			this.buttonMapColorCode.Location = new System.Drawing.Point(225, 48);
			this.buttonMapColorCode.Name = "buttonMapColorCode";
			this.buttonMapColorCode.Size = new System.Drawing.Size(74, 23);
			this.buttonMapColorCode.TabIndex = 18;
			this.buttonMapColorCode.Text = "Associer";
			this.buttonMapColorCode.UseVisualStyleBackColor = true;
			this.buttonMapColorCode.Click += new System.EventHandler(this.buttonMapColorCode_Click);
			// 
			// labelColorCodeNotFound
			// 
			this.labelColorCodeNotFound.AutoSize = true;
			this.labelColorCodeNotFound.Location = new System.Drawing.Point(8, 53);
			this.labelColorCodeNotFound.Name = "labelColorCodeNotFound";
			this.labelColorCodeNotFound.Size = new System.Drawing.Size(132, 13);
			this.labelColorCodeNotFound.TabIndex = 17;
			this.labelColorCodeNotFound.Text = "0 code couleur non trouvé";
			// 
			// buttonMapFormat
			// 
			this.buttonMapFormat.Enabled = false;
			this.buttonMapFormat.Location = new System.Drawing.Point(225, 78);
			this.buttonMapFormat.Name = "buttonMapFormat";
			this.buttonMapFormat.Size = new System.Drawing.Size(74, 23);
			this.buttonMapFormat.TabIndex = 16;
			this.buttonMapFormat.Text = "Associer";
			this.buttonMapFormat.UseVisualStyleBackColor = true;
			this.buttonMapFormat.Click += new System.EventHandler(this.buttonMapFormat_Click);
			// 
			// labelFormatNotFound
			// 
			this.labelFormatNotFound.AutoSize = true;
			this.labelFormatNotFound.Location = new System.Drawing.Point(8, 83);
			this.labelFormatNotFound.Name = "labelFormatNotFound";
			this.labelFormatNotFound.Size = new System.Drawing.Size(99, 13);
			this.labelFormatNotFound.TabIndex = 15;
			this.labelFormatNotFound.Text = "0 format non trouvé";
			// 
			// groupBox1
			// 
			this.groupBox1.Controls.Add(this.buttonImporterBaseCodipress);
			this.groupBox1.Controls.Add(this.checkBoxIgnoreDiffusionNotNATIONAL);
			this.groupBox1.Controls.Add(this.checkBoxIgnoreTypePubliciteNotCOMMERCIALE);
			this.groupBox1.Controls.Add(this.checkBoxIgnoreRubriques);
			this.groupBox1.Location = new System.Drawing.Point(12, 246);
			this.groupBox1.Name = "groupBox1";
			this.groupBox1.Size = new System.Drawing.Size(618, 95);
			this.groupBox1.TabIndex = 5;
			this.groupBox1.TabStop = false;
			this.groupBox1.Text = "Options";
			// 
			// buttonImporterBaseCodipress
			// 
			this.buttonImporterBaseCodipress.Location = new System.Drawing.Point(438, 20);
			this.buttonImporterBaseCodipress.Name = "buttonImporterBaseCodipress";
			this.buttonImporterBaseCodipress.Size = new System.Drawing.Size(174, 23);
			this.buttonImporterBaseCodipress.TabIndex = 3;
			this.buttonImporterBaseCodipress.Text = "Importer Base Codipress";
			this.buttonImporterBaseCodipress.UseVisualStyleBackColor = true;
			this.buttonImporterBaseCodipress.Click += new System.EventHandler(this.buttonImporterBaseCodipress_Click);
			// 
			// checkBoxIgnoreDiffusionNotNATIONAL
			// 
			this.checkBoxIgnoreDiffusionNotNATIONAL.AutoSize = true;
			this.checkBoxIgnoreDiffusionNotNATIONAL.Checked = true;
			this.checkBoxIgnoreDiffusionNotNATIONAL.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxIgnoreDiffusionNotNATIONAL.Location = new System.Drawing.Point(7, 68);
			this.checkBoxIgnoreDiffusionNotNATIONAL.Name = "checkBoxIgnoreDiffusionNotNATIONAL";
			this.checkBoxIgnoreDiffusionNotNATIONAL.Size = new System.Drawing.Size(169, 17);
			this.checkBoxIgnoreDiffusionNotNATIONAL.TabIndex = 2;
			this.checkBoxIgnoreDiffusionNotNATIONAL.Text = "Ignore Diffusion != NATIONAL";
			this.checkBoxIgnoreDiffusionNotNATIONAL.UseVisualStyleBackColor = true;
			this.checkBoxIgnoreDiffusionNotNATIONAL.CheckedChanged += new System.EventHandler(this.checkBoxIgnoreDiffusionNotNATIONAL_CheckedChanged);
			// 
			// checkBoxIgnoreTypePubliciteNotCOMMERCIALE
			// 
			this.checkBoxIgnoreTypePubliciteNotCOMMERCIALE.AutoSize = true;
			this.checkBoxIgnoreTypePubliciteNotCOMMERCIALE.Checked = true;
			this.checkBoxIgnoreTypePubliciteNotCOMMERCIALE.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxIgnoreTypePubliciteNotCOMMERCIALE.Location = new System.Drawing.Point(7, 44);
			this.checkBoxIgnoreTypePubliciteNotCOMMERCIALE.Name = "checkBoxIgnoreTypePubliciteNotCOMMERCIALE";
			this.checkBoxIgnoreTypePubliciteNotCOMMERCIALE.Size = new System.Drawing.Size(219, 17);
			this.checkBoxIgnoreTypePubliciteNotCOMMERCIALE.TabIndex = 1;
			this.checkBoxIgnoreTypePubliciteNotCOMMERCIALE.Text = "Ignore Type Publicite != COMMERCIALE";
			this.checkBoxIgnoreTypePubliciteNotCOMMERCIALE.UseVisualStyleBackColor = true;
			this.checkBoxIgnoreTypePubliciteNotCOMMERCIALE.CheckedChanged += new System.EventHandler(this.checkBoxIgnoreTypePubliciteNotCOMMERCIALE_CheckedChanged);
			// 
			// checkBoxIgnoreRubriques
			// 
			this.checkBoxIgnoreRubriques.AutoSize = true;
			this.checkBoxIgnoreRubriques.Checked = true;
			this.checkBoxIgnoreRubriques.CheckState = System.Windows.Forms.CheckState.Checked;
			this.checkBoxIgnoreRubriques.Location = new System.Drawing.Point(7, 20);
			this.checkBoxIgnoreRubriques.Name = "checkBoxIgnoreRubriques";
			this.checkBoxIgnoreRubriques.Size = new System.Drawing.Size(107, 17);
			this.checkBoxIgnoreRubriques.TabIndex = 0;
			this.checkBoxIgnoreRubriques.Text = "Ignore Rubriques";
			this.checkBoxIgnoreRubriques.UseVisualStyleBackColor = true;
			this.checkBoxIgnoreRubriques.CheckedChanged += new System.EventHandler(this.checkBoxIgnoreRubriques_CheckedChanged);
			// 
			// labelStatusImportCodipress
			// 
			this.labelStatusImportCodipress.AutoSize = true;
			this.labelStatusImportCodipress.Location = new System.Drawing.Point(7, 63);
			this.labelStatusImportCodipress.Name = "labelStatusImportCodipress";
			this.labelStatusImportCodipress.Size = new System.Drawing.Size(0, 13);
			this.labelStatusImportCodipress.TabIndex = 6;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(644, 392);
			this.Controls.Add(this.groupBox1);
			this.Controls.Add(this.groupBoxEtape3);
			this.Controls.Add(this.groupBoxEtape2);
			this.Controls.Add(this.progressBarSource);
			this.Controls.Add(this.groupBoxEtape1);
			this.Name = "Form1";
			this.Text = "Tarifs Presse";
			this.Load += new System.EventHandler(this.Form1_Load);
			this.groupBoxEtape1.ResumeLayout(false);
			this.groupBoxEtape1.PerformLayout();
			this.groupBoxEtape2.ResumeLayout(false);
			this.groupBoxEtape2.PerformLayout();
			this.groupBoxEtape3.ResumeLayout(false);
			this.groupBoxEtape3.PerformLayout();
			this.groupBox1.ResumeLayout(false);
			this.groupBox1.PerformLayout();
			this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxEtape1;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button buttonPathTarifs;
        private System.Windows.Forms.TextBox pathTarifs;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button buttonPathVersions;
        private System.Windows.Forms.TextBox pathVersions;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button buttonPathSupports;
        private System.Windows.Forms.TextBox pathSupports;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Button buttonPathParutions;
        private System.Windows.Forms.TextBox pathParutions;
        private System.Windows.Forms.GroupBox groupBoxEtape2;
        private System.Windows.Forms.ProgressBar progressBarSource;
        private System.Windows.Forms.Label labelEtape1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button buttonPathCodipress;
		private System.Windows.Forms.TextBox pathCodipress;
        private System.Windows.Forms.Button buttonMajBases;
        private System.Windows.Forms.Label labelSupportsNotFound;
        private System.Windows.Forms.Button buttonMapSupports;
        private System.Windows.Forms.GroupBox groupBoxEtape3;
        private System.Windows.Forms.Button buttonMapColorCode;
        private System.Windows.Forms.Label labelColorCodeNotFound;
        private System.Windows.Forms.Button buttonMapFormat;
        private System.Windows.Forms.Label labelFormatNotFound;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.CheckBox checkBoxIgnoreTypePubliciteNotCOMMERCIALE;
        private System.Windows.Forms.CheckBox checkBoxIgnoreRubriques;
		private System.Windows.Forms.CheckBox checkBoxIgnoreDiffusionNotNATIONAL;
		private System.Windows.Forms.Button buttonImporterBaseCodipress;
		private System.Windows.Forms.Label labelStatusImportCodipress;
    }
}

