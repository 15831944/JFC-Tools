namespace ATR_Report
{
    partial class ATR_Form
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ATR_Form));
            this.lst_region = new System.Windows.Forms.ListBox();
            this.lbl_region = new System.Windows.Forms.Label();
            this.lst_station = new System.Windows.Forms.ListBox();
            this.btn_ok = new System.Windows.Forms.Button();
            this.lbl_station = new System.Windows.Forms.Label();
            this.btn_get_report = new System.Windows.Forms.Button();
            this.bfr_region_code = new System.Windows.Forms.Label();
            this.bfr_slct_station_nb = new System.Windows.Forms.Label();
            this.bfr_result = new System.Windows.Forms.Label();
            this.bfr_status = new System.Windows.Forms.Label();
            this.bfr_dates = new System.Windows.Forms.Label();
            this.txt_search = new System.Windows.Forms.TextBox();
            this.lst_slct_station = new System.Windows.Forms.ListBox();
            this.btn_select_station = new System.Windows.Forms.Button();
            this.btn_cancel_station = new System.Windows.Forms.Button();
            this.chckbox_invalide_station = new System.Windows.Forms.CheckBox();
            this.pct_magnifier = new System.Windows.Forms.PictureBox();
            this.bfr_status_0 = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pct_magnifier)).BeginInit();
            this.SuspendLayout();
            // 
            // lst_region
            // 
            this.lst_region.FormattingEnabled = true;
            this.lst_region.ItemHeight = 15;
            this.lst_region.Location = new System.Drawing.Point(142, 38);
            this.lst_region.Name = "lst_region";
            this.lst_region.Size = new System.Drawing.Size(524, 94);
            this.lst_region.TabIndex = 1;
            this.lst_region.SelectedIndexChanged += new System.EventHandler(this.lstRegionSelectedIndexChanged);
            // 
            // lbl_region
            // 
            this.lbl_region.AutoSize = true;
            this.lbl_region.BackColor = System.Drawing.Color.Transparent;
            this.lbl_region.Location = new System.Drawing.Point(51, 38);
            this.lbl_region.Name = "lbl_region";
            this.lbl_region.Size = new System.Drawing.Size(80, 15);
            this.lbl_region.TabIndex = 3;
            this.lbl_region.Text = "Select region:";
            // 
            // lst_station
            // 
            this.lst_station.FormattingEnabled = true;
            this.lst_station.ItemHeight = 15;
            this.lst_station.Location = new System.Drawing.Point(58, 247);
            this.lst_station.Name = "lst_station";
            this.lst_station.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.lst_station.Size = new System.Drawing.Size(285, 184);
            this.lst_station.TabIndex = 4;
            this.lst_station.DoubleClick += new System.EventHandler(this.lstStationDoubleClick);
            // 
            // btn_ok
            // 
            this.btn_ok.Location = new System.Drawing.Point(579, 446);
            this.btn_ok.Name = "btn_ok";
            this.btn_ok.Size = new System.Drawing.Size(87, 32);
            this.btn_ok.TabIndex = 5;
            this.btn_ok.Text = "ok";
            this.btn_ok.UseVisualStyleBackColor = true;
            this.btn_ok.Click += new System.EventHandler(this.btnOkClick);
            // 
            // lbl_station
            // 
            this.lbl_station.AutoSize = true;
            this.lbl_station.BackColor = System.Drawing.Color.Transparent;
            this.lbl_station.Location = new System.Drawing.Point(55, 183);
            this.lbl_station.Name = "lbl_station";
            this.lbl_station.Size = new System.Drawing.Size(83, 15);
            this.lbl_station.TabIndex = 6;
            this.lbl_station.Text = "Select station:";
            // 
            // btn_get_report
            // 
            this.btn_get_report.Location = new System.Drawing.Point(58, 446);
            this.btn_get_report.Name = "btn_get_report";
            this.btn_get_report.Size = new System.Drawing.Size(608, 32);
            this.btn_get_report.TabIndex = 7;
            this.btn_get_report.Text = "get report";
            this.btn_get_report.UseVisualStyleBackColor = true;
            this.btn_get_report.Visible = false;
            this.btn_get_report.Click += new System.EventHandler(this.btnGetReportClick);
            // 
            // label3
            // 
            this.bfr_region_code.AutoSize = true;
            this.bfr_region_code.Location = new System.Drawing.Point(27, 133);
            this.bfr_region_code.Name = "label3";
            this.bfr_region_code.Size = new System.Drawing.Size(0, 15);
            this.bfr_region_code.TabIndex = 8;
            this.bfr_region_code.Visible = false;
            // 
            // bfr_slct_station_nb
            // 
            this.bfr_slct_station_nb.AutoSize = true;
            this.bfr_slct_station_nb.Location = new System.Drawing.Point(30, 339);
            this.bfr_slct_station_nb.Name = "bfr_slct_station_nb";
            this.bfr_slct_station_nb.Size = new System.Drawing.Size(0, 15);
            this.bfr_slct_station_nb.TabIndex = 9;
            this.bfr_slct_station_nb.Visible = false;
            // 
            // bfr_result
            // 
            this.bfr_result.AutoSize = true;
            this.bfr_result.Location = new System.Drawing.Point(652, 226);
            this.bfr_result.Name = "bfr_result";
            this.bfr_result.Size = new System.Drawing.Size(14, 15);
            this.bfr_result.TabIndex = 10;
            this.bfr_result.Text = "0";
            this.bfr_result.Visible = false;
            // 
            // bfr_status
            // 
            this.bfr_status.AutoSize = true;
            this.bfr_status.Location = new System.Drawing.Point(735, 455);
            this.bfr_status.Name = "bfr_status";
            this.bfr_status.Size = new System.Drawing.Size(0, 15);
            this.bfr_status.TabIndex = 11;
            this.bfr_status.Visible = false;
            // 
            // bfr_dates
            // 
            this.bfr_dates.AutoSize = true;
            this.bfr_dates.Location = new System.Drawing.Point(737, 456);
            this.bfr_dates.Name = "bfr_dates";
            this.bfr_dates.Size = new System.Drawing.Size(0, 15);
            this.bfr_dates.TabIndex = 12;
            this.bfr_dates.Visible = false;
            // 
            // txt_search
            // 
            this.txt_search.Location = new System.Drawing.Point(58, 218);
            this.txt_search.Name = "txt_search";
            this.txt_search.Size = new System.Drawing.Size(185, 23);
            this.txt_search.TabIndex = 13;
            this.txt_search.Text = "Search...";
            this.txt_search.Click += new System.EventHandler(this.txtSearchClick);
            this.txt_search.TextChanged += new System.EventHandler(this.txtSearchChanged);
            // 
            // lst_slct_station
            // 
            this.lst_slct_station.FormattingEnabled = true;
            this.lst_slct_station.ItemHeight = 15;
            this.lst_slct_station.Location = new System.Drawing.Point(381, 247);
            this.lst_slct_station.Name = "lst_slct_station";
            this.lst_slct_station.SelectionMode = System.Windows.Forms.SelectionMode.MultiExtended;
            this.lst_slct_station.Size = new System.Drawing.Size(285, 184);
            this.lst_slct_station.TabIndex = 14;
            // 
            // btn_select_station
            // 
            this.btn_select_station.Location = new System.Drawing.Point(347, 273);
            this.btn_select_station.Name = "btn_select_station";
            this.btn_select_station.Size = new System.Drawing.Size(29, 26);
            this.btn_select_station.TabIndex = 15;
            this.btn_select_station.Text = ">";
            this.btn_select_station.UseVisualStyleBackColor = true;
            this.btn_select_station.Click += new System.EventHandler(this.btnSelectStationClick);
            // 
            // btn_cancel_station
            // 
            this.btn_cancel_station.Location = new System.Drawing.Point(347, 360);
            this.btn_cancel_station.Name = "btn_cancel_station";
            this.btn_cancel_station.Size = new System.Drawing.Size(29, 26);
            this.btn_cancel_station.TabIndex = 16;
            this.btn_cancel_station.Text = "<";
            this.btn_cancel_station.UseVisualStyleBackColor = true;
            this.btn_cancel_station.Click += new System.EventHandler(this.btnCancelStationClick);
            // 
            // chckbox_invalide_station
            // 
            this.chckbox_invalide_station.AutoSize = true;
            this.chckbox_invalide_station.BackColor = System.Drawing.Color.Transparent;
            this.chckbox_invalide_station.Location = new System.Drawing.Point(259, 218);
            this.chckbox_invalide_station.Name = "chckbox_invalide_station";
            this.chckbox_invalide_station.Size = new System.Drawing.Size(140, 19);
            this.chckbox_invalide_station.TabIndex = 17;
            this.chckbox_invalide_station.Text = "Hide invalide station";
            this.chckbox_invalide_station.UseVisualStyleBackColor = false;
            this.chckbox_invalide_station.CheckedChanged += new System.EventHandler(this.chckBoxCheckedChanged);
            // 
            // pct_magnifier
            // 
            this.pct_magnifier.BackColor = System.Drawing.Color.Transparent;
            this.pct_magnifier.Image = ((System.Drawing.Image)(resources.GetObject("pct_magnifier.Image")));
            this.pct_magnifier.Location = new System.Drawing.Point(29, 217);
            this.pct_magnifier.Name = "pct_magnifier";
            this.pct_magnifier.Size = new System.Drawing.Size(28, 26);
            this.pct_magnifier.TabIndex = 18;
            this.pct_magnifier.TabStop = false;
            // 
            // bfr_status_0
            // 
            this.bfr_status_0.AutoSize = true;
            this.bfr_status_0.Location = new System.Drawing.Point(624, 404);
            this.bfr_status_0.Name = "bfr_status_0";
            this.bfr_status_0.Size = new System.Drawing.Size(42, 15);
            this.bfr_status_0.TabIndex = 19;
            this.bfr_status_0.Text = "label8";
            this.bfr_status_0.Visible = false;
            // 
            // ATR_Form
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(685, 515);
            this.Controls.Add(this.pct_magnifier);
            this.Controls.Add(this.chckbox_invalide_station);
            this.Controls.Add(this.btn_cancel_station);
            this.Controls.Add(this.btn_select_station);
            this.Controls.Add(this.lst_slct_station);
            this.Controls.Add(this.txt_search);
            this.Controls.Add(this.bfr_dates);
            this.Controls.Add(this.bfr_status);
            this.Controls.Add(this.bfr_result);
            this.Controls.Add(this.bfr_slct_station_nb);
            this.Controls.Add(this.bfr_region_code);
            this.Controls.Add(this.btn_get_report);
            this.Controls.Add(this.lbl_station);
            this.Controls.Add(this.btn_ok);
            this.Controls.Add(this.lst_station);
            this.Controls.Add(this.lbl_region);
            this.Controls.Add(this.lst_region);
            this.Controls.Add(this.bfr_status_0);
            this.DoubleBuffered = true;
            this.Font = new System.Drawing.Font("Calibri", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
            this.ForeColor = System.Drawing.SystemColors.ControlText;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ATR_Form";
            this.Text = "ATRREPORT";
            this.Load += new System.EventHandler(this.ATRFormLoad);
            ((System.ComponentModel.ISupportInitialize)(this.pct_magnifier)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListBox lst_region;
        private System.Windows.Forms.Label lbl_region;
        private System.Windows.Forms.ListBox lst_station;
        private System.Windows.Forms.Button btn_ok;
        private System.Windows.Forms.Label lbl_station;
        private System.Windows.Forms.Button btn_get_report;
        private System.Windows.Forms.Label bfr_region_code;
        private System.Windows.Forms.Label bfr_slct_station_nb;
        private System.Windows.Forms.Label bfr_result;
        private System.Windows.Forms.Label bfr_status;
        private System.Windows.Forms.Label bfr_dates;
        private System.Windows.Forms.TextBox txt_search;
        private System.Windows.Forms.ListBox lst_slct_station;
        private System.Windows.Forms.Button btn_select_station;
        private System.Windows.Forms.Button btn_cancel_station;
        private System.Windows.Forms.CheckBox chckbox_invalide_station;
        private System.Windows.Forms.PictureBox pct_magnifier;
        private System.Windows.Forms.Label bfr_status_0;


    }
}