// JFCGridControl.JFCGridColumnHeaderSort
using JFCGridControl;
using System.Windows.Controls.Primitives;

namespace JFCGridControl
{
    public class JFCGridColumnHeaderSort : ButtonBase
    {
        private JFCGridColumnHeader header;

        public JFCGridColumnHeader Header
        {
            get
            {
                return this.header;
            }
        }

        public JFCGridColumnHeaderSort(JFCGridColumnHeader header)
        {
            base.ClipToBounds = true;
            base.IsTabStop = false;
            base.Focusable = false;
            this.header = header;
        }
    }
}