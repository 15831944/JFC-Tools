using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls.Primitives;

namespace JFCGridControl
{
    public class JFCGridColumnHeaderSort : ButtonBase
    {
        public JFCGridColumnHeaderSort(JFCGridColumnHeader header)
        {
            this.ClipToBounds = true;
            this.IsTabStop = false;
            this.Focusable = false;

            this.header = header;
        }

        private JFCGridColumnHeader header = null;
        public JFCGridColumnHeader Header
        {
            get { return header; }
        }
    }
}
