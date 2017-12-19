using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Media;
using System.Windows;

namespace JFCGridControl
{
    public abstract class JFCTileBrush : TileBrush
    {

        protected override void GetContentBounds(out Rect contentBounds)
        {
            throw new NotImplementedException();
        }

        protected override Freezable CreateInstanceCore()
        {
            throw new NotImplementedException();
        }
    }
}
