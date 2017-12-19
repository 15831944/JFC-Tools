// JFCGridControl.JFCGridCell
using JFCGridControl;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;


namespace JFCGridControl
{
    public class JFCGridCell : Control
    {
        public static readonly DependencyProperty HiliteBrushProperty = DependencyProperty.Register("HiliteColor", typeof(Brush), typeof(JFCGridCell), new UIPropertyMetadata(new SolidColorBrush(Colors.Red)));

        public static readonly DependencyProperty HiliteTextProperty = DependencyProperty.Register("HiliteText", typeof(string), typeof(JFCGridCell), new UIPropertyMetadata(""));

        private JFCGridItem itemRow;

        public static readonly DependencyProperty IsMouseOverLineItemProperty = DependencyProperty.Register("IsMouseOverLineItem", typeof(bool), typeof(JFCGridCell), new UIPropertyMetadata(false, JFCGridCell.UpdateIsMouseOverLine));

        private JFCGridItem itemColumn;

        public static readonly DependencyProperty IsMouseOverColumnItemProperty = DependencyProperty.Register("IsMouseOverColumnItem", typeof(bool), typeof(JFCGridCell), new UIPropertyMetadata(false, JFCGridCell.UpdateIsMouseOverColumn));

        public Brush HiliteBrush
        {
            get
            {
                return (Brush)base.GetValue(JFCGridCell.HiliteBrushProperty);
            }
            set
            {
                base.SetValue(JFCGridCell.HiliteBrushProperty, value);
            }
        }

        public string HiliteText
        {
            get
            {
                return (string)base.GetValue(JFCGridCell.HiliteTextProperty);
            }
            set
            {
                base.SetValue(JFCGridCell.HiliteTextProperty, value);
            }
        }

        public JFCGridItem ItemRow
        {
            get
            {
                return this.itemRow;
            }
            set
            {
                if (this.itemRow != null)
                {
                    this.itemRow.IsMouseOverLineChanged -= this.item_IsMouseOverLineChanged;
                }
                this.itemRow = value;
                this.itemRow.IsMouseOverLineChanged += this.item_IsMouseOverLineChanged;
            }
        }

        public bool IsMouseOverLineItem
        {
            get
            {
                return (bool)base.GetValue(JFCGridCell.IsMouseOverLineItemProperty);
            }
            set
            {
                base.SetValue(JFCGridCell.IsMouseOverLineItemProperty, value);
            }
        }

        public JFCGridItem ItemColumn
        {
            get
            {
                return this.itemColumn;
            }
            set
            {
                if (this.itemColumn != null)
                {
                    this.itemColumn.IsMouseOverColumnChanged -= this.item_IsMouseOverColumnChanged;
                }
                this.itemColumn = value;
                this.itemColumn.IsMouseOverColumnChanged += this.item_IsMouseOverColumnChanged;
            }
        }

        public bool IsMouseOverColumnItem
        {
            get
            {
                return (bool)base.GetValue(JFCGridCell.IsMouseOverColumnItemProperty);
            }
            set
            {
                base.SetValue(JFCGridCell.IsMouseOverColumnItemProperty, value);
            }
        }

        public event RoutedEventHandler IsMouseOverLineChanged;

        public event RoutedEventHandler IsMouseOverColumnChanged;

        public JFCGridCell()
        {
            base.ClipToBounds = true;
            base.SnapsToDevicePixels = true;
            base.IsTabStop = false;
            base.Focusable = false;
        }

        private void item_IsMouseOverLineChanged(object sender, RoutedEventArgs e)
        {
            this.IsMouseOverLineItem = this.itemRow.IsMouseOverLine;
        }

        internal void OnIsMouseOverLineChanged(object sender, RoutedEventArgs e)
        {
            if (this.IsMouseOverLineChanged != null)
            {
                this.IsMouseOverLineChanged(this, e);
            }
        }

        private static void UpdateIsMouseOverLine(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGridCell obj2 = obj as JFCGridCell;
            obj2.OnIsMouseOverLineChanged(obj2, new RoutedEventArgs());
        }

        private void item_IsMouseOverColumnChanged(object sender, RoutedEventArgs e)
        {
        }

        internal void OnIsMouseOverColumnChanged(object sender, RoutedEventArgs e)
        {
            if (this.IsMouseOverColumnChanged != null)
            {
                this.IsMouseOverColumnChanged(this, e);
            }
        }

        private static void UpdateIsMouseOverColumn(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGridCell obj2 = obj as JFCGridCell;
            obj2.OnIsMouseOverColumnChanged(obj2, new RoutedEventArgs());
        }
    }
}