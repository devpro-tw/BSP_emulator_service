namespace TestCOMForm
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
            this.btnInit = new System.Windows.Forms.Button();
            this.edParams = new System.Windows.Forms.TextBox();
            this.txtAction = new System.Windows.Forms.TextBox();
            this.txtParams = new System.Windows.Forms.TextBox();
            this.btnExecService = new System.Windows.Forms.Button();
            this.edTerminalID = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // btnInit
            // 
            this.btnInit.Location = new System.Drawing.Point(318, 32);
            this.btnInit.Name = "btnInit";
            this.btnInit.Size = new System.Drawing.Size(108, 23);
            this.btnInit.TabIndex = 0;
            this.btnInit.Text = "Init";
            this.btnInit.UseVisualStyleBackColor = true;
            this.btnInit.Click += new System.EventHandler(this.button1_Click);
            // 
            // edParams
            // 
            this.edParams.Location = new System.Drawing.Point(107, 32);
            this.edParams.Name = "edParams";
            this.edParams.Size = new System.Drawing.Size(122, 22);
            this.edParams.TabIndex = 1;
            this.edParams.Text = "C:\\TN3270NF";
            // 
            // txtAction
            // 
            this.txtAction.Location = new System.Drawing.Point(107, 107);
            this.txtAction.Name = "txtAction";
            this.txtAction.Size = new System.Drawing.Size(122, 22);
            this.txtAction.TabIndex = 2;
            this.txtAction.Text = "PCIC-2";
            // 
            // txtParams
            // 
            this.txtParams.Location = new System.Drawing.Point(107, 144);
            this.txtParams.Name = "txtParams";
            this.txtParams.Size = new System.Drawing.Size(122, 22);
            this.txtParams.TabIndex = 3;
            this.txtParams.Text = "ID=A123123123";
            // 
            // btnExecService
            // 
            this.btnExecService.Location = new System.Drawing.Point(318, 71);
            this.btnExecService.Name = "btnExecService";
            this.btnExecService.Size = new System.Drawing.Size(108, 23);
            this.btnExecService.TabIndex = 4;
            this.btnExecService.Text = "Exec Service";
            this.btnExecService.UseVisualStyleBackColor = true;
            // 
            // edTerminalID
            // 
            this.edTerminalID.Location = new System.Drawing.Point(107, 70);
            this.edTerminalID.Name = "edTerminalID";
            this.edTerminalID.Size = new System.Drawing.Size(122, 22);
            this.edTerminalID.TabIndex = 5;
            this.edTerminalID.Text = "A";
            this.edTerminalID.TextChanged += new System.EventHandler(this.edTerminalID_TextChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(486, 206);
            this.Controls.Add(this.edTerminalID);
            this.Controls.Add(this.btnExecService);
            this.Controls.Add(this.txtParams);
            this.Controls.Add(this.txtAction);
            this.Controls.Add(this.edParams);
            this.Controls.Add(this.btnInit);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnInit;
        private System.Windows.Forms.TextBox edParams;
        private System.Windows.Forms.TextBox txtAction;
        private System.Windows.Forms.TextBox txtParams;
        private System.Windows.Forms.Button btnExecService;
        private System.Windows.Forms.TextBox edTerminalID;
    }
}

