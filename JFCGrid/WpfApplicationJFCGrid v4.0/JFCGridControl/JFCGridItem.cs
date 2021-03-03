using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Shapes;
using System.Windows;
using System.Windows.Media;
using System.ComponentModel;
using System.Windows.Controls;
using System.Windows.Input;

namespace JFCGridControl
{
    public class JFCGridItem : ContentControl
    {

        public JFCGridItem(JFCGrid parent)
        {
            this.ClipToBounds = true;
            this.SnapsToDevicePixels = true;
            this.Focusable = false;

            this.Peer = new List<JFCGridItem>();

            this.ParentGrid = parent;
        }

        public JFCGrid ParentGrid
        {
            get;
            set;
        }

        public List<JFCGridItem> Peer
        {
            get;
            set;
        }

        public void SelectedPeer(bool isSelected)
        {
            foreach (var peer in Peer)
            {
                peer.IsSelected = isSelected;
            }
        }

        public int IndexData
        {
            get;
            set;
        }

        protected override void OnMouseLeftButtonDown(MouseButtonEventArgs e)
        {
            base.OnMouseEnter(e);
        }

        public void MouseEnterItem()
        {
            if (this.Orientation == System.Windows.Controls.Orientation.Horizontal)
            {
                Boolean IsMouseOver = (Boolean)this.GetValue(JFCGridItem.IsMouseOverLineProperty);

                if (IsMouseOver != true)
                {

                    this.SetValue(JFCGridItem.IsMouseOverLineProperty, true);

                    if (Peer != null)
                    {
                        foreach (var peer in Peer)
                        {
                            peer.CallIsMouseOverLineCallBack = false;
                            peer.SetValue(JFCGridItem.IsMouseOverLineProperty, true);
                            peer.CallIsMouseOverLineCallBack = true;
                        }
                    }

                }
            }
            else if (this.Orientation == System.Windows.Controls.Orientation.Vertical)
            {
                //Boolean IsMouseOver = (Boolean)this.GetValue(JFCGridItem.IsMouseOverColumnProperty);

                //if (IsMouseOver != true)
                //{
                //    this.SetValue(JFCGridItem.IsMouseOverColumnProperty, true);

                //    if (Peer != null)
                //    {
                //        foreach (var peer in Peer)
                //        {
                //            peer.CallIsMouseOverColumnCallBack = false;
                //            peer.SetValue(JFCGridItem.IsMouseOverColumnProperty, true);
                //            peer.CallIsMouseOverColumnCallBack = true;
                //        }
                //    }
                //}
            }
        }

        public void MouseLeaveItem()
        {
            if (this.Orientation == System.Windows.Controls.Orientation.Horizontal)
            {
                Boolean IsMouseOver = (Boolean)this.GetValue(JFCGridItem.IsMouseOverLineProperty);

                if (IsMouseOver != false)
                {

                    this.SetValue(JFCGridItem.IsMouseOverLineProperty, false);

                    if (Peer != null)
                    {
                        foreach (var peer in Peer)
                        {
                            peer.CallIsMouseOverLineCallBack = false;
                            peer.SetValue(JFCGridItem.IsMouseOverLineProperty, false);
                            peer.CallIsMouseOverLineCallBack = true;
                        }
                    }
                }
            }
            else if (this.Orientation == System.Windows.Controls.Orientation.Vertical)
            {
                //this.SetValue(JFCGridItem.IsMouseOverColumnProperty, false);

                //if (Peer != null)
                //{
                //    foreach (var peer in Peer)
                //    {
                //        peer.CallIsMouseOverColumnCallBack = false;
                //        peer.SetValue(JFCGridItem.IsMouseOverColumnProperty, false);
                //        peer.CallIsMouseOverColumnCallBack = true;
                //    }
                //}
            }
        }


        protected override void OnMouseEnter(MouseEventArgs e)
        {
            base.OnMouseEnter(e);

            if (this.Orientation == System.Windows.Controls.Orientation.Horizontal)
            {
                this.SetValue(JFCGridItem.IsMouseOverLineProperty, true);

                if (Peer != null)
                {
                    foreach (var peer in Peer)
                    {
                        peer.CallIsMouseOverLineCallBack = false;
                        peer.SetValue(JFCGridItem.IsMouseOverLineProperty, true);
                        peer.CallIsMouseOverLineCallBack = true;
                    }
                }
            }
            else if (this.Orientation == System.Windows.Controls.Orientation.Vertical)
            {
                //this.SetValue(JFCGridItem.IsMouseOverColumnProperty, true);

                //if (Peer != null)
                //{
                //    foreach (var peer in Peer)
                //    {
                //        peer.CallIsMouseOverColumnCallBack = false;
                //        peer.SetValue(JFCGridItem.IsMouseOverColumnProperty, true);
                //        peer.CallIsMouseOverColumnCallBack = true;
                //    }
                //}
            }
        }

        protected override void OnMouseLeave(MouseEventArgs e)
        {
            base.OnMouseLeave(e);

            if (this.Orientation == System.Windows.Controls.Orientation.Horizontal)
            {
                this.SetValue(JFCGridItem.IsMouseOverLineProperty, false);

                if (Peer != null)
                {
                    foreach (var peer in Peer)
                    {
                        peer.CallIsMouseOverLineCallBack = false;
                        peer.SetValue(JFCGridItem.IsMouseOverLineProperty, false);
                        peer.CallIsMouseOverLineCallBack = true;
                    }
                }
            }
            else if (this.Orientation == System.Windows.Controls.Orientation.Vertical)
            {
                //this.SetValue(JFCGridItem.IsMouseOverColumnProperty, false);

                //if (Peer != null)
                //{
                //    foreach (var peer in Peer)
                //    {
                //        peer.CallIsMouseOverColumnCallBack = false;
                //        peer.SetValue(JFCGridItem.IsMouseOverColumnProperty, false);
                //        peer.CallIsMouseOverColumnCallBack = true;
                //    }
                //}
            }
        }

        internal Boolean CallIsMouseOverLineCallBack = true;

        public Boolean IsMouseOverLine
        {
            get { return (Boolean)GetValue(IsMouseOverLineProperty); }
            set { SetValue(IsMouseOverLineProperty, value); }
        }

        // Using a DependencyProperty as the backing store for IsMouseOverLine.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IsMouseOverLineProperty =
            DependencyProperty.Register("IsMouseOverLine", typeof(Boolean), typeof(JFCGridItem), new UIPropertyMetadata(false, new PropertyChangedCallback(UpdateIsMouseOverLine)));

        internal Boolean CallIsMouseOverColumnCallBack = true;

        public Boolean IsMouseOverColumn
        {
            get { return (Boolean)GetValue(IsMouseOverColumnProperty); }
            set { SetValue(IsMouseOverColumnProperty, value); }
        }

        // Using a DependencyProperty as the backing store for IsMouseOverColumn.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IsMouseOverColumnProperty =
            DependencyProperty.Register("IsMouseOverColumn", typeof(Boolean), typeof(JFCGridItem), new UIPropertyMetadata(false, new PropertyChangedCallback(UpdateIsMouseOverColumn)));


        public Boolean IsSelected
        {
            get { return (Boolean)GetValue(IsSelectedProperty); }
            set { SetValue(IsSelectedProperty, value); }
        }

        // Using a DependencyProperty as the backing store for IsSelected.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IsSelectedProperty =
            DependencyProperty.Register("IsSelected", typeof(Boolean), typeof(JFCGridItem), new UIPropertyMetadata(false));


        public Boolean IsAlternate
        {
            get { return (Boolean)GetValue(IsAlternateProperty); }
            set { SetValue(IsAlternateProperty, value); }
        }

        // Using a DependencyProperty as the backing store for IsAlternate.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty IsAlternateProperty =
            DependencyProperty.Register("IsAlternate", typeof(Boolean), typeof(JFCGridItem), new UIPropertyMetadata(false));

        //public Orientation Orientation;
        public Orientation Orientation
        {
            get { return (Orientation)GetValue(OrientationProperty); }
            set { SetValue(OrientationProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Orientation.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty OrientationProperty =
            DependencyProperty.Register("Orientation", typeof(Orientation), typeof(JFCGridItem), new UIPropertyMetadata(Orientation.Horizontal, new PropertyChangedCallback(UpdateOrientation)));


        private static void UpdateOrientation(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGridItem Item = obj as JFCGridItem;

            if (Item.Orientation == Orientation.Vertical)
            {
                Item.MouseMove += new MouseEventHandler(Item_MouseMove);
            }
            else
            {
                Item.MouseMove -= Item_MouseMove;
            }
        }


        static void Item_MouseMove(object sender, MouseEventArgs e)
        {
            JFCGridItem it = sender as JFCGridItem;

            if (it.ParentGrid != null)
            {
                Point pt = e.GetPosition(it);

                double modulo = (pt.Y % it.ParentGrid.Manager.Parent.RowHeight);
                double row = (pt.Y - modulo) / it.ParentGrid.Manager.Parent.RowHeight;

                if (modulo > 0)
                    row++;

                int index = 0;

                foreach (var lst in it.ParentGrid.Manager.TabLineB)
                {
                    var l = from ctrl in lst
                            where ctrl is JFCGridItem
                            select ctrl;

                    var le = from ctrl in l.Cast<JFCGridItem>()
                             where ctrl.Orientation == Orientation.Horizontal
                             select ctrl;

                    if (le.Count() > 0)
                        if (index == (int)row - 1)
                        {
                            //((JFCGridItem)le.First()).SetValue(JFCGridItem.IsMouseOverLineProperty, true);
                            ((JFCGridItem)le.First()).MouseEnterItem();
                        }
                        else
                        {
                            //((JFCGridItem)le.First()).SetValue(JFCGridItem.IsMouseOverLineProperty, false);
                            ((JFCGridItem)le.First()).MouseLeaveItem();
                        }

                    index++;
                }


                //if (it.ParentGrid.Manager.TabLineB.Count() > (int)row - 1)
                //{
                //    var lst = it.ParentGrid.Manager.TabLineB.ElementAt(((int)row) - 1);

                //    var l = from ctrl in lst
                //            where ctrl is JFCGridItem
                //            select ctrl;

                //    var le = from ctrl in l.Cast<JFCGridItem>()
                //             where ctrl.Orientation == Orientation.Horizontal
                //             select ctrl;

                //    if (le.Count() > 0)
                //        ((JFCGridItem)le.First()).SetValue(JFCGridItem.IsMouseOverLineProperty, true);
                //}
            }
        }

        private JFCExpendItem data = null;
        public JFCExpendItem Data
        {
            get { return data; }
            set
            {
                data = value;
                if (data != null)
                    Source = data.Obj;
                else
                    Source = null;
            }
        }

        public object Source
        {
            get { return (object)GetValue(SourceProperty); }
            private set { SetValue(SourceProperty, value); }
        }

        // Using a DependencyProperty as the backing store for Source.  This enables animation, styling, binding, etc...
        public static readonly DependencyProperty SourceProperty =
            DependencyProperty.Register("Source", typeof(object), typeof(JFCGridItem), new UIPropertyMetadata("test"));

        // 
        public event RoutedEventHandler IsMouseOverLineChanged;
        internal void OnIsMouseOverLineChanged(object sender, RoutedEventArgs e)
        {
            if (IsMouseOverLineChanged != null)
                IsMouseOverLineChanged(this, e);
        }

        private static void UpdateIsMouseOverLine(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGridItem Item = obj as JFCGridItem;

            if (Item.CallIsMouseOverLineCallBack == true)
            {
                Item.OnIsMouseOverLineChanged(Item, new RoutedEventArgs());
            }
        }

        // 
        public event RoutedEventHandler IsMouseOverColumnChanged;
        internal void OnIsMouseOverColumnChanged(object sender, RoutedEventArgs e)
        {
            if (IsMouseOverColumnChanged != null)
                IsMouseOverColumnChanged(this, e);
        }

        private static void UpdateIsMouseOverColumn(DependencyObject obj, DependencyPropertyChangedEventArgs e)
        {
            JFCGridItem Item = obj as JFCGridItem;

            if (Item.CallIsMouseOverColumnCallBack == true)
            {
                Item.OnIsMouseOverColumnChanged(Item, new RoutedEventArgs());
            }
        }

    }
}
