// JFCGridControl.JFCGridColumnHeader
using JFCGridControl;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;


namespace JFCGridControl
{
    public class JFCGridColumnHeader : ButtonBase
    {
        public enum SortType
        {
            None,
            Ascending,
            Descending
        }

        public enum InsertType
        {
            None,
            Left,
            Top,
            Right,
            Bottom
        }

        public static readonly DependencyProperty HeaderSortProperty = DependencyProperty.Register("HeaderSort", typeof(JFCGridColumnHeaderSort), typeof(JFCGridColumnHeader), new UIPropertyMetadata(null));

        internal JFCGridColumnBelowHeader BHeader;

        public static readonly DependencyProperty ColumnProperty = DependencyProperty.Register("Column", typeof(JFCGridColumn), typeof(JFCGridColumnHeader), new UIPropertyMetadata(null));

        public static readonly DependencyProperty IsSortProperty = DependencyProperty.Register("IsSort", typeof(SortType), typeof(JFCGridColumnHeader), new UIPropertyMetadata(SortType.None));

        public static readonly DependencyProperty OrientationProperty = DependencyProperty.Register("Orientation", typeof(double), typeof(JFCGridColumnHeader), new UIPropertyMetadata(0.0));

        public static readonly DependencyProperty IsInsertProperty = DependencyProperty.Register("IsInsert", typeof(InsertType), typeof(JFCGridColumnHeader), new UIPropertyMetadata(InsertType.None));

        public JFCGridColumnHeaderSort HeaderSort
        {
            get
            {
                return (JFCGridColumnHeaderSort)base.GetValue(JFCGridColumnHeader.HeaderSortProperty);
            }
            set
            {
                base.SetValue(JFCGridColumnHeader.HeaderSortProperty, value);
            }
        }

        public JFCGridColumnBelowHeader BelowHeader
        {
            get
            {
                return this.BHeader;
            }
        }

        public JFCGridColumn Column
        {
            get
            {
                return (JFCGridColumn)base.GetValue(JFCGridColumnHeader.ColumnProperty);
            }
            internal set
            {
                base.SetValue(JFCGridColumnHeader.ColumnProperty, value);
            }
        }

        public SortType IsSort
        {
            get
            {
                return (SortType)base.GetValue(JFCGridColumnHeader.IsSortProperty);
            }
            set
            {
                base.SetValue(JFCGridColumnHeader.IsSortProperty, value);
            }
        }

        public double Orientation
        {
            get
            {
                return (double)base.GetValue(JFCGridColumnHeader.OrientationProperty);
            }
            set
            {
                base.SetValue(JFCGridColumnHeader.OrientationProperty, value);
            }
        }

        public InsertType IsInsert
        {
            get
            {
                return (InsertType)base.GetValue(JFCGridColumnHeader.IsInsertProperty);
            }
            set
            {
                base.SetValue(JFCGridColumnHeader.IsInsertProperty, value);
            }
        }

        public event RoutedEventHandler ClickSort;

        public event DependencyPropertyChangedEventHandler ContentChanged;

        public JFCGridColumnHeader()
        {
            base.ClipToBounds = true;
            base.IsTabStop = false;
            base.Focusable = false;
            this.BHeader = new JFCGridColumnBelowHeader(this);
            this.HeaderSort = new JFCGridColumnHeaderSort(this);
            this.HeaderSort.Click += this.HeaderSort_Click;
        }

        internal void OnClickSort(object sender, RoutedEventArgs e)
        {
            if (this.ClickSort != null)
            {
                this.ClickSort(this, e);
            }
        }

        private void HeaderSort_Click(object sender, RoutedEventArgs e)
        {
            this.OnClickSort(sender, e);
        }

        protected override void OnContentChanged(object oldContent, object newContent)
        {
            base.OnContentChanged(oldContent, newContent);
            DependencyPropertyChangedEventArgs e = new DependencyPropertyChangedEventArgs(ContentControl.ContentProperty, oldContent, newContent);
            this.OnContentChanged(this, e);
        }

        internal void OnContentChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (this.ContentChanged != null)
            {
                this.ContentChanged(this, e);
            }
        }

        protected override void OnPropertyChanged(DependencyPropertyChangedEventArgs e)
        {
            base.OnPropertyChanged(e);
            if (e.Property == FrameworkElement.ContextMenuProperty && this.BHeader != null)
            {
                this.BHeader.ContextMenu = (e.NewValue as ContextMenu);
            }
        }
    }
}