using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Controls;
using System.Windows;
using System.ComponentModel;
using System.Windows.Media;

namespace JFCGridControl
{
    public class JFCGridCell : Control //CellImg //TextBlock, INotifyPropertyChanged //CellImg //ContentControl //TextBlock //ContentControl
    {
        public Brush HiliteBrush
        {
            get { return (Brush)GetValue(HiliteBrushProperty); }
            set { SetValue(HiliteBrushProperty, value); }
        }

        // Using a DependencyProperty as the backing store for HiliteColor.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty HiliteBrushProperty =
            DependencyProperty.Register("HiliteColor", typeof(Brush), typeof(JFCGridCell), new UIPropertyMetadata(new SolidColorBrush(Colors.Red)));


        public String HiliteText
        {
            get { return (String)GetValue(HiliteTextProperty); }
            set { SetValue(HiliteTextProperty, value); }
        }

        // Using a DependencyProperty as the backing store for HiliteText.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty HiliteTextProperty =
            DependencyProperty.Register("HiliteText", typeof(String), typeof(JFCGridCell), new UIPropertyMetadata(""));


        public JFCGridCell()
        {
            this.ClipToBounds = true;
            this.SnapsToDevicePixels = true;
            this.IsTabStop = false;
            this.Focusable = false;

        }

        #region ItemRow
        
        private JFCGridItem itemRow = null;
        public JFCGridItem ItemRow
        {
            get { return itemRow; }
            set
            {
                if (itemRow != null)
                    itemRow.IsMouseOverLineChanged -= item_IsMouseOverLineChanged;

                itemRow = value;
                itemRow.IsMouseOverLineChanged += new RoutedEventHandler(item_IsMouseOverLineChanged);
            }
        }

        void item_IsMouseOverLineChanged(object sender, RoutedEventArgs e)
        {
            IsMouseOverLineItem = itemRow.IsMouseOverLine;
        }

        public bool IsMouseOverLineItem
        {
            get { return (bool)GetValue(IsMouseOverLineItemProperty); }
            set { SetValue(IsMouseOverLineItemProperty, value); }
        }

        // Using a DependencyProperty as the backing store for IsMouseOverLineItem.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IsMouseOverLineItemProperty =
            DependencyProperty.Register("IsMouseOverLineItem", typeof(bool), typeof(JFCGridCell), new UIPropertyMetadata(false, new PropertyChangedCallback(UpdateIsMouseOverLine)));

        public event RoutedEventHandler IsMouseOverLineChanged;
        internal void OnIsMouseOverLineChanged(object sender, RoutedEventArgs e)
        {
            if (IsMouseOverLineChanged != null)
                IsMouseOverLineChanged(this, e);
        }

        private static void UpdateIsMouseOverLine(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGridCell cell = obj as JFCGridCell;

            cell.OnIsMouseOverLineChanged(cell, new RoutedEventArgs());
        }

        #endregion

        #region ItemColumn

        private JFCGridItem itemColumn = null;
        public JFCGridItem ItemColumn
        {
            get { return itemColumn; }
            set
            {
                if (itemColumn != null)
                    itemColumn.IsMouseOverColumnChanged -= item_IsMouseOverColumnChanged;

                itemColumn = value;
                itemColumn.IsMouseOverColumnChanged += new RoutedEventHandler(item_IsMouseOverColumnChanged);
            }
        }

        void item_IsMouseOverColumnChanged(object sender, RoutedEventArgs e)
        {
            //IsMouseOverColumnItem = itemRow.IsMouseOverColumn;
        }

        public bool IsMouseOverColumnItem
        {
            get { return (bool)GetValue(IsMouseOverColumnItemProperty); }
            set { SetValue(IsMouseOverColumnItemProperty, value); }
        }

        // Using a DependencyProperty as the backing store for IsMouseOverColumnItem.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IsMouseOverColumnItemProperty =
            DependencyProperty.Register("IsMouseOverColumnItem", typeof(bool), typeof(JFCGridCell), new UIPropertyMetadata(false, new PropertyChangedCallback(UpdateIsMouseOverColumn)));

        public event RoutedEventHandler IsMouseOverColumnChanged;
        internal void OnIsMouseOverColumnChanged(object sender, RoutedEventArgs e)
        {
            if (IsMouseOverColumnChanged != null)
                IsMouseOverColumnChanged(this, e);
        }

        private static void UpdateIsMouseOverColumn(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGridCell cell = obj as JFCGridCell;

            cell.OnIsMouseOverColumnChanged(cell, new RoutedEventArgs());
        }

        #endregion

    }
}
