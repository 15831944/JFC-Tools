using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls.Primitives;
using System.Windows;
using System.Windows.Media;

namespace JFCGridControl
{
    public class JFCGridColumnHeader : ButtonBase
    {
        public JFCGridColumnHeader()
        {
            this.ClipToBounds = true;
            this.IsTabStop = false;
            this.Focusable = false;

            BHeader = new JFCGridColumnBelowHeader(this);
            HeaderSort = new JFCGridColumnHeaderSort(this);
            
            HeaderSort.Click += new RoutedEventHandler(HeaderSort_Click);
                    
        }       

        public event RoutedEventHandler ClickSort;
        internal void OnClickSort(object sender, RoutedEventArgs e)
        {
            if (ClickSort != null)
                ClickSort(this, e);
        }
        
        public JFCGridColumnHeaderSort HeaderSort
        {
            get { return (JFCGridColumnHeaderSort)GetValue(HeaderSortProperty); }
            set { SetValue(HeaderSortProperty, value); }
        }

        // Using a DependencyProperty as the backing store for HeaderSort.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty HeaderSortProperty =
            DependencyProperty.Register("HeaderSort", typeof(JFCGridColumnHeaderSort), typeof(JFCGridColumnHeader), new UIPropertyMetadata(null));

        

        void HeaderSort_Click(object sender, RoutedEventArgs e)
        {
            OnClickSort(sender, e);

            //e.Handled = true;
        }

        internal JFCGridColumnBelowHeader BHeader;

        public JFCGridColumnBelowHeader BelowHeader
        {
            get { return BHeader; }
        }

        public JFCGridColumn Column
        {
            get { return (JFCGridColumn)GetValue(ColumnProperty); }
            internal set { SetValue(ColumnProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Column.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty ColumnProperty =
            DependencyProperty.Register("Column", typeof(JFCGridColumn), typeof(JFCGridColumnHeader), new UIPropertyMetadata(null));


        public enum SortType
        {
            None,
            Ascending,
            Descending
        }

        public SortType IsSort
        {
            get { return (SortType)GetValue(IsSortProperty); }
            set { SetValue(IsSortProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Sort.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IsSortProperty = DependencyProperty.Register("IsSort", typeof(SortType), typeof(JFCGridColumnHeader), new UIPropertyMetadata(SortType.None));


        public double Orientation
        {
            get { return (double)GetValue(OrientationProperty); }
            set { SetValue(OrientationProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Orientation.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty OrientationProperty =
            DependencyProperty.Register("Orientation", typeof(double), typeof(JFCGridColumnHeader), new UIPropertyMetadata(0.0));

        public enum InsertType
        {
            None,
            Left,
            Top,
            Right,
            Bottom
        }

        public InsertType IsInsert
        {
            get { return (InsertType)GetValue(IsInsertProperty); }
            set { SetValue(IsInsertProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Sort.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IsInsertProperty = DependencyProperty.Register("IsInsert", typeof(InsertType), typeof(JFCGridColumnHeader), new UIPropertyMetadata(InsertType.None));
        

        protected override void OnContentChanged(object oldContent, object newContent)
        {
            base.OnContentChanged(oldContent, newContent);

            DependencyPropertyChangedEventArgs e = new DependencyPropertyChangedEventArgs(JFCGridColumnHeader.ContentProperty, oldContent, newContent);

            OnContentChanged(this, e);
        }

        public event DependencyPropertyChangedEventHandler ContentChanged;
        internal void OnContentChanged(object sender, DependencyPropertyChangedEventArgs e)
        {
            if (ContentChanged != null)
                ContentChanged(this, e);
        }

        protected override void OnPropertyChanged(DependencyPropertyChangedEventArgs e)
        {
            base.OnPropertyChanged(e);


            if (e.Property == JFCGridColumnHeader.ContextMenuProperty)
            {
                if (BHeader != null)
                    BHeader.ContextMenu = e.NewValue as System.Windows.Controls.ContextMenu;
            }

        }

        


		  
	
    }
}
