// JFCGridControl.JFCGridManager
using JFCGridControl;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Input;
using System.Windows.Markup;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.Xml;


namespace JFCGridControl
{
    internal class JFCGridManager
    {
        public enum ApplyMode
        {
            View,
            CalculSize
        }

        private JFCGridControl.ObservableCollection<JFCGridColumn> ListColBody;

        private List<JFCGridColumn> ListColHeaderRow;

        private List<JFCGridColumn> ListColHeaderFooter;

        private List<JFCGridColumn> ListColHeaderRowEnd;

        private Grid Grid00;

        private Grid Grid01;

        private Grid Grid02;

        private Grid Grid10;

        private Grid Grid11;

        private Grid Grid12;

        private Grid Grid20;

        private Grid Grid21;

        internal JFCGrid Parent;

        private Rectangle HeaderMove = new Rectangle();

        private Point PtHeaderMove;

        private Rectangle ColDestinationIndicator;

        private int MaxLevelColumn;

        internal List<LinkedList<UIElement>> TabLineH = new List<LinkedList<UIElement>>();

        internal List<LinkedList<UIElement>> TabLineHF = new List<LinkedList<UIElement>>();

        internal List<LinkedList<UIElement>> TabLineB = new List<LinkedList<UIElement>>();

        internal List<LinkedList<UIElement>> TabLineBF = new List<LinkedList<UIElement>>();

        internal List<LinkedList<UIElement>> TabLineF = new List<LinkedList<UIElement>>();

        private List<CellImg> LstRecycleCellImg = new List<CellImg>();

        private List<JFCGridCell> LstRecycleJFCGridCell = new List<JFCGridCell>();

        private List<CellImg> LstRemoveColCellImg = new List<CellImg>();

        private List<JFCGridCell> LstRemoveColJFCGridCell = new List<JFCGridCell>();

        private List<UIElement> LstRemoveHeader = new List<UIElement>();

        private List<UIElement> LstRemoveBelowHeader = new List<UIElement>();

        private List<UIElement> LstRemoveSplitter = new List<UIElement>();

        private DispatcherTimer timer;

        private bool InProcess;

        private bool UseLstRemoveCol;

        private int startIndexColView;

        private int nbColView;

        public int StartIndexColView
        {
            get
            {
                return this.startIndexColView;
            }
            set
            {
                if (!this.InProcess)
                {
                    this.InProcess = true;
                    if (this.startIndexColView != value)
                    {
                        int num = this.startIndexColView;
                        this.startIndexColView = value;
                        if (this.NbColView > 0)
                        {
                            GridLength gridLength;
                            if (num < value)
                            {
                                for (int i = num; i < value; i++)
                                {
                                    if (this.Grid11.ColumnDefinitions.Count() > 0)
                                    {
                                        this.ColumnRemove(0, this.Grid11, this.TabLineB, this.UseLstRemoveCol);
                                        this.ColumnRemove(0, this.Grid21, this.TabLineBF);
                                        this.ColumnRemoveGridHeader(this.Grid01, 0);
                                    }
                                }
                                for (int j = this.Grid11.ColumnDefinitions.Count(); j < this.NbColView && value + j < this.ListColBody.Count(); j++)
                                {
                                    JFCGridColumn jFCGridColumn = ((Collection<JFCGridColumn>)this.ListColBody)[value + j];
                                    this.ColumnAddGrid(jFCGridColumn, j, this.Grid11, this.TabLineB, this.ListColBody);
                                    this.ColumnAddGridFooter(jFCGridColumn, j, this.Grid21, this.TabLineBF, this.ListColBody);
                                    this.ColumnAddGridHeader(this.Grid01, jFCGridColumn, j);
                                    gridLength = jFCGridColumn.Width;
                                    if (gridLength.IsAuto)
                                    {
                                        gridLength = jFCGridColumn.ActualWidth;
                                        if (gridLength.IsAuto)
                                        {
                                            jFCGridColumn.ActualWidth = this.CalculTailleMaxi(jFCGridColumn);
                                        }
                                    }
                                }
                                this.ClearLstRemoveCol();
                            }
                            else if (num > value)
                            {
                                int num2 = this.Grid11.ColumnDefinitions.Count();
                                if (num + num2 != value + this.nbColView)
                                {
                                    if (num + num2 > value + this.nbColView)
                                    {
                                        int num3 = num + num2 - (value + this.nbColView);
                                        int num4 = 0;
                                        while (num4 < num3)
                                        {
                                            if (this.Grid11.ColumnDefinitions.Count() > 0)
                                            {
                                                this.ColumnRemove(num2 - 1, this.Grid11, this.TabLineB, this.UseLstRemoveCol);
                                                this.ColumnRemove(num2 - 1, this.Grid21, this.TabLineBF);
                                                this.ColumnRemoveGridHeader(this.Grid01, num2 - 1);
                                            }
                                            num4++;
                                            num2--;
                                        }
                                    }
                                    else
                                    {
                                        int num5 = num2;
                                        while (num5 > 0)
                                        {
                                            if (this.Grid11.ColumnDefinitions.Count() > 0)
                                            {
                                                this.ColumnRemove(num2 - 1, this.Grid11, this.TabLineB, this.UseLstRemoveCol);
                                                this.ColumnRemove(num2 - 1, this.Grid21, this.TabLineBF);
                                                this.ColumnRemoveGridHeader(this.Grid01, num2 - 1);
                                            }
                                            num5++;
                                            num2--;
                                        }
                                    }
                                    num2 = this.Grid11.ColumnDefinitions.Count();
                                    for (int k = 0; k < this.NbColView - num2 && value + k < this.ListColBody.Count(); k++)
                                    {
                                        JFCGridColumn jFCGridColumn2 = ((Collection<JFCGridColumn>)this.ListColBody)[value + k];
                                        this.ColumnAddGrid(jFCGridColumn2, k, this.Grid11, this.TabLineB, this.ListColBody);
                                        this.ColumnAddGridFooter(jFCGridColumn2, k, this.Grid21, this.TabLineBF, this.ListColBody);
                                        this.ColumnAddGridHeader(this.Grid01, jFCGridColumn2, k);
                                        gridLength = jFCGridColumn2.Width;
                                        if (gridLength.IsAuto)
                                        {
                                            gridLength = jFCGridColumn2.ActualWidth;
                                            if (gridLength.IsAuto)
                                            {
                                                gridLength = jFCGridColumn2.ActualWidth;
                                                if (gridLength.IsAuto)
                                                {
                                                    jFCGridColumn2.ActualWidth = this.CalculTailleMaxi(jFCGridColumn2);
                                                }
                                            }
                                        }
                                    }
                                    this.ClearLstRemoveCol();
                                }
                                else
                                {
                                    num2 = this.Grid11.ColumnDefinitions.Count();
                                    for (int l = 0; l < this.NbColView - num2 && value + l < this.ListColBody.Count(); l++)
                                    {
                                        JFCGridColumn jFCGridColumn3 = ((Collection<JFCGridColumn>)this.ListColBody)[value + l];
                                        this.ColumnAddGrid(jFCGridColumn3, l, this.Grid11, this.TabLineB, this.ListColBody);
                                        this.ColumnAddGridFooter(jFCGridColumn3, l, this.Grid21, this.TabLineBF, this.ListColBody);
                                        this.ColumnAddGridHeader(this.Grid01, jFCGridColumn3, l);
                                        gridLength = jFCGridColumn3.Width;
                                        if (gridLength.IsAuto)
                                        {
                                            gridLength = jFCGridColumn3.ActualWidth;
                                            if (gridLength.IsAuto)
                                            {
                                                gridLength = jFCGridColumn3.ActualWidth;
                                                if (gridLength.IsAuto)
                                                {
                                                    jFCGridColumn3.ActualWidth = this.CalculTailleMaxi(jFCGridColumn3);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        this.ReBuildColumnHeader(JFCGridColumn.FrozenType.None);
                    }
                    this.InProcess = false;
                }
                if (!this.InProcess)
                {
                    return;
                }
                throw new Exception();
            }
        }

        public int NbColView
        {
            get
            {
                return this.nbColView;
            }
            set
            {
                if (!this.InProcess)
                {
                    this.InProcess = true;
                    GridLength gridLength;
                    if (value == 0)
                    {
                        for (int num = this.Grid11.ColumnDefinitions.Count() - 1; num >= 0; num--)
                        {
                            this.ColumnRemove(num, this.Grid11, this.TabLineB);
                            this.ColumnRemove(num, this.Grid21, this.TabLineBF);
                            this.ColumnRemoveGridHeader(this.Grid01, num);
                        }
                        this.nbColView = value;
                    }
                    else if (this.nbColView != value)
                    {
                        int num2 = this.nbColView;
                        this.nbColView = value;
                        if (value > num2)
                        {
                            int num3 = num2;
                            for (int i = num2; i < value && this.StartIndexColView + i < this.ListColBody.Count(); i++)
                            {
                                JFCGridColumn jFCGridColumn = ((Collection<JFCGridColumn>)this.ListColBody)[this.StartIndexColView + i];
                                this.ColumnAddGrid(jFCGridColumn, num3, this.Grid11, this.TabLineB, this.ListColBody);
                                this.ColumnAddGridFooter(jFCGridColumn, num3, this.Grid21, this.TabLineBF, this.ListColBody);
                                this.ColumnAddGridHeader(this.Grid01, jFCGridColumn, num3);
                                gridLength = jFCGridColumn.Width;
                                if (gridLength.IsAuto)
                                {
                                    gridLength = jFCGridColumn.ActualWidth;
                                    if (gridLength.IsAuto)
                                    {
                                        jFCGridColumn.ActualWidth = this.CalculTailleMaxi(jFCGridColumn);
                                    }
                                }
                                num3++;
                            }
                        }
                        else if (value < num2)
                        {
                            for (int num4 = this.Grid11.ColumnDefinitions.Count() - 1; num4 >= value; num4--)
                            {
                                if (this.StartIndexColView + num4 < this.ListColBody.Count() && num4 < this.Grid11.ColumnDefinitions.Count())
                                {
                                    this.ColumnRemove(num4, this.Grid11, this.TabLineB);
                                    this.ColumnRemove(num4, this.Grid21, this.TabLineBF);
                                    this.ColumnRemoveGridHeader(this.Grid01, num4);
                                }
                            }
                        }
                        this.ReBuildColumnHeader(JFCGridColumn.FrozenType.None);
                    }
                    else if (this.Grid11.ColumnDefinitions.Count() != this.NbColView)
                    {
                        if (this.NbColView > this.Grid11.ColumnDefinitions.Count())
                        {
                            for (int j = this.Grid11.ColumnDefinitions.Count(); j < this.NbColView && this.StartIndexColView + j < this.ListColBody.Count(); j++)
                            {
                                JFCGridColumn jFCGridColumn2 = ((Collection<JFCGridColumn>)this.ListColBody)[this.StartIndexColView + j];
                                this.ColumnAddGrid(jFCGridColumn2, j, this.Grid11, this.TabLineB, this.ListColBody);
                                this.ColumnAddGridFooter(jFCGridColumn2, j, this.Grid21, this.TabLineBF, this.ListColBody);
                                this.ColumnAddGridHeader(this.Grid01, jFCGridColumn2, j);
                                gridLength = jFCGridColumn2.Width;
                                if (gridLength.IsAuto)
                                {
                                    gridLength = jFCGridColumn2.ActualWidth;
                                    if (gridLength.IsAuto)
                                    {
                                        jFCGridColumn2.ActualWidth = this.CalculTailleMaxi(jFCGridColumn2);
                                    }
                                }
                            }
                        }
                        else if (this.NbColView < this.Grid11.ColumnDefinitions.Count())
                        {
                            for (int num5 = this.Grid11.ColumnDefinitions.Count(); num5 > this.NbColView; num5--)
                            {
                                if (this.StartIndexColView + num5 < this.ListColBody.Count())
                                {
                                    JFCGridColumn jFCGridColumn3 = ((Collection<JFCGridColumn>)this.ListColBody)[this.StartIndexColView + num5 - 1];
                                    this.ColumnRemove(num5 - 1, this.Grid11, this.TabLineB);
                                    this.ColumnRemove(num5 - 1, this.Grid21, this.TabLineBF);
                                    this.ColumnRemoveGridHeader(this.Grid01, num5 - 1);
                                }
                            }
                        }
                        this.ReBuildColumnHeader(JFCGridColumn.FrozenType.None);
                    }
                    this.InProcess = false;
                }
                if (!this.InProcess)
                {
                    return;
                }
                throw new Exception();
            }
        }

        public void SetStartIndexAndNbCol(int IdxStart, int NbCol)
        {
            if (this.startIndexColView != IdxStart)
            {
                this.nbColView = NbCol;
                this.StartIndexColView = IdxStart;
            }
            else if (this.nbColView != NbCol)
            {
                this.startIndexColView = IdxStart;
                this.NbColView = NbCol;
            }
        }

        public JFCGridManager(JFCGrid Parent)
        {
            this.Grid00 = Parent.Grid00;
            this.Grid01 = Parent.Grid01;
            this.Grid02 = Parent.Grid02;
            this.Grid10 = Parent.Grid10;
            this.Grid11 = Parent.Grid11;
            this.Grid12 = Parent.Grid12;
            this.Grid20 = Parent.Grid20;
            this.Grid21 = Parent.Grid21;
            this.Parent = Parent;
            this.ListColBody = new JFCGridControl.ObservableCollection<JFCGridColumn>();
            this.ListColBody.CollectionChanged += this.ListColBody_CollectionChanged;
            this.ListColHeaderRow = new List<JFCGridColumn>();
            this.ListColHeaderFooter = new List<JFCGridColumn>();
            this.ListColHeaderRowEnd = new List<JFCGridColumn>();
            this.ColDestinationIndicator = new Rectangle();
            this.ColDestinationIndicator.HorizontalAlignment = HorizontalAlignment.Left;
            this.ColDestinationIndicator.VerticalAlignment = VerticalAlignment.Stretch;
            this.ColDestinationIndicator.Width = 5.0;
            this.ColDestinationIndicator.Fill = Brushes.Red;
            this.timer = new DispatcherTimer();
        }

        private void ListColBody_CollectionChanged(object sender, NotifyCollectionChangedEventArgs e)
        {
            GridLength gridLength;
            if (this.Parent.IsVirtualised)
            {
                if (e.Action == NotifyCollectionChangedAction.Add)
                {
                    if (this.NbColView == 0)
                    {
                        foreach (JFCGridColumn newItem in e.NewItems)
                        {
                            gridLength = newItem.Width;
                            if (gridLength.IsAuto)
                            {
                                gridLength = newItem.ActualWidth;
                                if (gridLength.IsAuto)
                                {
                                    newItem.ActualWidth = this.CalculTailleMaxi(newItem);
                                }
                            }
                        }
                        return;
                    }
                    int num = 0;
                    foreach (JFCGridColumn newItem2 in e.NewItems)
                    {
                        int num2 = e.NewStartingIndex + num;
                        if (this.StartIndexColView <= num2 && num2 <= this.StartIndexColView + this.NbColView - 1)
                        {
                            if (this.StartIndexColView + this.NbColView < this.ListColBody.Count())
                            {
                                int num3 = this.StartIndexColView + this.Grid11.ColumnDefinitions.Count() - 1;
                                if (num3 < this.Grid11.ColumnDefinitions.Count())
                                {
                                    this.ColumnRemove(num3, this.Grid11, this.TabLineB, this.UseLstRemoveCol);
                                    this.ColumnRemove(num3, this.Grid21, this.TabLineBF);
                                    this.ColumnRemoveGridHeader(this.Grid01, num3);
                                }
                                this.ClearParentLstRemoveCol();
                            }
                            this.ColumnAddGrid(newItem2, num2 - this.StartIndexColView, this.Grid11, this.TabLineB, this.ListColBody);
                            this.ColumnAddGridFooter(newItem2, num2 - this.StartIndexColView, this.Grid21, this.TabLineBF, this.ListColBody);
                            this.ColumnAddGridHeader(this.Grid01, newItem2, num2 - this.StartIndexColView);
                            gridLength = newItem2.Width;
                            if (gridLength.IsAuto)
                            {
                                gridLength = newItem2.ActualWidth;
                                if (gridLength.IsAuto)
                                {
                                    newItem2.ActualWidth = this.CalculTailleMaxi(newItem2);
                                }
                            }
                        }
                        num++;
                    }
                    this.ClearLstRemoveCol();
                }
                else if (e.Action == NotifyCollectionChangedAction.Move)
                {
                    if (this.StartIndexColView <= e.OldStartingIndex && e.OldStartingIndex <= this.StartIndexColView + this.NbColView - 1)
                    {
                        int index = e.OldStartingIndex - this.StartIndexColView;
                        this.ColumnRemove(index, this.Grid11, this.TabLineB, this.UseLstRemoveCol);
                        this.ColumnRemove(index, this.Grid21, this.TabLineBF);
                        this.ColumnRemoveGridHeader(this.Grid01, index);
                    }
                    this.ClearParentLstRemoveCol();
                    if (this.StartIndexColView <= e.NewStartingIndex && e.NewStartingIndex <= this.StartIndexColView + this.NbColView - 1)
                    {
                        JFCGridColumn column = ((Collection<JFCGridColumn>)this.ListColBody)[e.NewStartingIndex];
                        int index2 = e.NewStartingIndex - this.StartIndexColView;
                        this.ColumnAddGrid(column, index2, this.Grid11, this.TabLineB, this.ListColBody);
                        this.ColumnAddGridFooter(column, index2, this.Grid21, this.TabLineBF, this.ListColBody);
                        this.ColumnAddGridHeader(this.Grid01, column, index2);
                    }
                    this.ClearLstRemoveCol();
                }
                else
                {
                    if (e.Action == NotifyCollectionChangedAction.Remove)
                    {
                        if (this.NbColView != 0)
                        {
                            int num4 = 0;
                            foreach (JFCGridColumn oldItem in e.OldItems)
                            {
                                int num5 = e.OldStartingIndex + num4;
                                if (this.StartIndexColView <= num5 && num5 <= this.StartIndexColView + this.NbColView - 1)
                                {
                                    num5 -= this.StartIndexColView;
                                    this.ColumnRemove(num5, this.Grid11, this.TabLineB, this.UseLstRemoveCol);
                                    this.ColumnRemove(num5, this.Grid21, this.TabLineBF);
                                    this.ColumnRemoveGridHeader(this.Grid01, num5);
                                    this.ClearParentLstRemoveCol();
                                    if (this.StartIndexColView + this.NbColView - 1 < this.ListColBody.Count())
                                    {
                                        JFCGridColumn column2 = ((Collection<JFCGridColumn>)this.ListColBody)[this.StartIndexColView + this.NbColView - 1];
                                        num5 = this.NbColView - 1;
                                        this.ColumnAddGrid(column2, num5, this.Grid11, this.TabLineB, this.ListColBody);
                                        this.ColumnAddGridFooter(column2, num5, this.Grid21, this.TabLineBF, this.ListColBody);
                                        this.ColumnAddGridHeader(this.Grid01, column2, num5);
                                        gridLength = oldItem.Width;
                                        if (gridLength.IsAuto)
                                        {
                                            gridLength = oldItem.ActualWidth;
                                            if (gridLength.IsAuto)
                                            {
                                                oldItem.ActualWidth = this.CalculTailleMaxi(oldItem);
                                            }
                                        }
                                    }
                                }
                                num4++;
                            }
                            goto IL_057e;
                        }
                        return;
                    }
                    if (e.Action != NotifyCollectionChangedAction.Replace && e.Action == NotifyCollectionChangedAction.Reset)
                    {
                        if (this.NbColView == 0)
                        {
                            return;
                        }
                        for (int num6 = this.Grid11.ColumnDefinitions.Count(); num6 > 0; num6--)
                        {
                            this.ColumnRemove(0, this.Grid11, this.TabLineB);
                            this.ColumnRemove(0, this.Grid21, this.TabLineBF);
                            this.ColumnRemoveGridHeader(this.Grid01, 0);
                        }
                        this.startIndexColView = 0;
                        if (this.Parent.IsVirtualised)
                        {
                            this.nbColView = 0;
                        }
                        this.Parent.ScrollBarVertical.Value = 0.0;
                    }
                }
                goto IL_057e;
            }
            if (e.Action == NotifyCollectionChangedAction.Add)
            {
                int num7 = e.NewStartingIndex;
                foreach (JFCGridColumn newItem3 in e.NewItems)
                {
                    this.ColumnAddGrid(newItem3, num7, this.Grid11, this.TabLineB, this.ListColBody);
                    this.ColumnAddGridFooter(newItem3, num7, this.Grid21, this.TabLineBF, this.ListColBody);
                    this.ColumnAddGridHeader(this.Grid01, newItem3, num7);
                    gridLength = newItem3.Width;
                    if (gridLength.IsAuto)
                    {
                        gridLength = newItem3.ActualWidth;
                        if (gridLength.IsAuto)
                        {
                            newItem3.ActualWidth = this.CalculTailleMaxi(newItem3);
                        }
                    }
                    else
                    {
                        gridLength = newItem3.Width;
                        bool isStar = gridLength.IsStar;
                    }
                    num7++;
                }
            }
            else if (e.Action == NotifyCollectionChangedAction.Move)
            {
                int oldStartingIndex = e.OldStartingIndex;
                this.ColumnRemove(oldStartingIndex, this.Grid11, this.TabLineB, this.UseLstRemoveCol);
                this.ColumnRemove(oldStartingIndex, this.Grid21, this.TabLineBF);
                this.ColumnRemoveGridHeader(this.Grid01, oldStartingIndex);
                this.ClearParentLstRemoveCol();
                JFCGridColumn column3 = ((Collection<JFCGridColumn>)this.ListColBody)[e.NewStartingIndex];
                oldStartingIndex = e.NewStartingIndex;
                this.ColumnAddGrid(column3, oldStartingIndex, this.Grid11, this.TabLineB, this.ListColBody);
                this.ColumnAddGridFooter(column3, oldStartingIndex, this.Grid21, this.TabLineBF, this.ListColBody);
                this.ColumnAddGridHeader(this.Grid01, column3, oldStartingIndex);
                this.ClearLstRemoveCol();
            }
            else if (e.Action == NotifyCollectionChangedAction.Remove)
            {
                int num8 = 0;
                foreach (JFCGridColumn oldItem2 in e.OldItems)
                {
                    JFCGridColumn jFCGridColumn = oldItem2;
                    int index3 = e.OldStartingIndex + num8;
                    this.ColumnRemove(index3, this.Grid11, this.TabLineB);
                    this.ColumnRemove(index3, this.Grid21, this.TabLineBF);
                    this.ColumnRemoveGridHeader(this.Grid01, index3);
                    num8++;
                }
            }
            else if (e.Action != NotifyCollectionChangedAction.Replace && e.Action == NotifyCollectionChangedAction.Reset)
            {
                for (int num9 = this.Grid11.ColumnDefinitions.Count(); num9 > 0; num9--)
                {
                    this.ColumnRemove(0, this.Grid11, this.TabLineB);
                    this.ColumnRemove(0, this.Grid21, this.TabLineBF);
                    this.ColumnRemoveGridHeader(this.Grid01, 0);
                }
            }
            this.startIndexColView = 0;
            if (this.Parent.IsVirtualised)
            {
                this.nbColView = 0;
            }
            this.ReBuildColumnHeader(JFCGridColumn.FrozenType.None);
            return;
        IL_057e:
            this.ReBuildColumnHeader(JFCGridColumn.FrozenType.None);
        }

        public void ColumnAdd(JFCGridColumn Column)
        {
            if (Column.Frozen == JFCGridColumn.FrozenType.Start)
            {
                int index = this.ListColHeaderRow.Count();
                this.ColumnAdd(Column, index);
            }
            else if (Column.Frozen == JFCGridColumn.FrozenType.End)
            {
                int index2 = this.ListColHeaderRowEnd.Count();
                this.ColumnAdd(Column, index2);
            }
            else
            {
                int index3 = this.ListColBody.Count();
                this.ColumnAdd(Column, index3);
            }
        }

        public void ColumnAdd(JFCGridColumn Column, int Index)
        {
            Column.PropertyChanged += this.Column_PropertyChanged;
            GridLength gridLength;
            if (Column.Frozen == JFCGridColumn.FrozenType.Start)
            {
                Grid grid = this.Grid00;
                this.ListColHeaderRow.Insert(Index, Column);
                this.ColumnAddGrid(Column, Index, this.Grid10, this.TabLineH, this.ListColHeaderRow);
                ColumnDefinition columnDefinition = new ColumnDefinition();
                grid.ColumnDefinitions.Insert(Index, columnDefinition);
                Binding binding = new Binding("ActualWidth");
                binding.Source = Column;
                binding.Mode = BindingMode.TwoWay;
                columnDefinition.SetBinding(ColumnDefinition.WidthProperty, binding);
                gridLength = Column.Width;
                if (gridLength.IsAuto)
                {
                    gridLength = Column.ActualWidth;
                    if (gridLength.IsAuto)
                    {
                        Column.ActualWidth = this.CalculTailleMaxi(Column);
                    }
                }
            }
            else if (Column.Frozen == JFCGridColumn.FrozenType.End)
            {
                Grid grid2 = this.Grid02;
                this.ListColHeaderRowEnd.Insert(Index, Column);
                this.ColumnAddGrid(Column, Index, this.Grid12, this.TabLineF, this.ListColHeaderRowEnd);
                ColumnDefinition columnDefinition2 = new ColumnDefinition();
                grid2.ColumnDefinitions.Insert(Index, columnDefinition2);
                Binding binding2 = new Binding("ActualWidth");
                binding2.Source = Column;
                binding2.Mode = BindingMode.TwoWay;
                columnDefinition2.SetBinding(ColumnDefinition.WidthProperty, binding2);
                gridLength = Column.Width;
                if (gridLength.IsAuto)
                {
                    gridLength = Column.ActualWidth;
                    if (gridLength.IsAuto)
                    {
                        Column.ActualWidth = this.CalculTailleMaxi(Column);
                    }
                }
            }
            else
            {
                this.ListColBody.Insert(Index, Column);
            }
        }

        public void RefreshSizeColumnStar(JFCGridColumn Column)
        {
            if (Column.Width.IsStar)
            {
                Column.ActualWidth = new GridLength(this.Parent.ScrollViewerBody.ActualWidth);
            }
        }

        public void RefreshSizeColumnStar()
        {
            foreach (JFCGridColumn item in this.ListColBody)
            {
                this.RefreshSizeColumnStar(item);
            }
        }

        public void ColumnAddHeaderFooter(JFCGridColumn Column)
        {
            int index = this.ListColHeaderFooter.Count();
            this.ColumnAddHeaderFooter(Column, index);
        }

        public void ColumnAddHeaderFooter(JFCGridColumn Column, int Index)
        {
            Column.PropertyChanged += this.Column_PropertyChanged;
            Grid grid = this.Grid20;
            this.ListColHeaderFooter.Insert(Index, Column);
            this.ColumnAddGridFooter(Column, Index, this.Grid20, this.TabLineHF, this.ListColHeaderFooter);
            int num = 0;
            int num2 = 0;
            int num3 = this.MaxLevelColumn = this.NbLevelColumn(this.Parent.ColunmDefinitionFooter, ref num, ref num2);
            grid.RowDefinitions.Clear();
            for (int i = 0; i < this.MaxLevelColumn; i++)
            {
                grid.RowDefinitions.Add(new RowDefinition());
            }
            num = 0;
            num2 = 0;
            GridLength gridLength = Column.Width;
            if (gridLength.IsAuto)
            {
                gridLength = Column.ActualWidth;
                if (gridLength.IsAuto)
                {
                    Column.ActualWidth = this.CalculTailleMaxi(Column);
                }
            }
        }

        private List<JFCExpendItem> FindChildren(JFCExpendItem item)
        {
            List<JFCExpendItem> list = new List<JFCExpendItem>();
            foreach (JFCExpendItem child in item.Children)
            {
                list.Add(child);
                list.AddRange(this.FindChildren(child));
            }
            return list;
        }

        private void SetupEvents(UIElement MyElement)
        {
            MyElement.MouseEnter -= this.MyElement_MouseEnter;
            MyElement.MouseEnter += this.MyElement_MouseEnter;
            MyElement.MouseLeave -= this.MyElement_MouseLeave;
            MyElement.MouseLeave += this.MyElement_MouseLeave;
            MyElement.MouseLeftButtonDown -= this.MyElement_MouseLeftButtonDown;
            MyElement.MouseLeftButtonDown += this.MyElement_MouseLeftButtonDown;
            MyElement.MouseLeftButtonUp -= this.MyElement_MouseLeftButtonUp;
            MyElement.MouseLeftButtonUp += this.MyElement_MouseLeftButtonUp;
            MyElement.MouseRightButtonUp -= this.MyElement_MouseRightButtonUp;
            MyElement.MouseRightButtonUp += this.MyElement_MouseRightButtonUp;
            MyElement.MouseDown -= this.MyElement_MouseDown;
            MyElement.MouseDown += this.MyElement_MouseDown;
        }

        private void DeleteEvents(UIElement MyElement, bool IsContentGrouping = false)
        {
            MyElement.MouseEnter -= this.MyElement_MouseEnter;
            MyElement.MouseLeave -= this.MyElement_MouseLeave;
            MyElement.MouseLeftButtonDown -= this.MyElement_MouseLeftButtonDown;
            MyElement.MouseLeftButtonUp -= this.MyElement_MouseLeftButtonUp;
            MyElement.MouseRightButtonUp -= this.MyElement_MouseRightButtonUp;
            MyElement.MouseDown -= this.MyElement_MouseDown;
            if (MyElement is CellImg && !IsContentGrouping)
            {
                ((CellImg)MyElement).ResetEvent();
            }
        }

        private void MyElement_MouseEnter(object sender, MouseEventArgs e)
        {
            FrameworkElement frameworkElement = sender as FrameworkElement;
            if (frameworkElement is JFCGridItem)
            {
                ((JFCGridItem)frameworkElement).MouseEnterItem();
            }
            else
            {
                List<LinkedList<UIElement>> list = null;
                IEnumerable<JFCGridColumn> enumerable = null;
                if (frameworkElement.Parent == this.Grid10)
                {
                    list = this.TabLineH;
                    enumerable = this.ListColHeaderRow;
                }
                else if (frameworkElement.Parent == this.Grid11)
                {
                    list = this.TabLineB;
                    enumerable = this.ListColBody;
                }
                else if (frameworkElement.Parent == this.Grid12)
                {
                    list = this.TabLineF;
                    enumerable = this.ListColHeaderRowEnd;
                }
                else if (frameworkElement.Parent == this.Grid20)
                {
                    list = this.TabLineHF;
                    enumerable = this.ListColHeaderFooter;
                }
                else if (frameworkElement.Parent == this.Grid21)
                {
                    list = this.TabLineBF;
                    enumerable = this.ListColBody;
                }
                if (list != null)
                {
                    bool flag = true;
                    if (frameworkElement is JFCBorder && (frameworkElement as JFCBorder).Orientation == Orientation.Vertical)
                    {
                        flag = false;
                    }
                    if (flag)
                    {
                        int index = (int)frameworkElement.GetValue(Grid.RowProperty);
                        IEnumerable<UIElement> enumerable2 = from ctrl in list[index]
                                                             where ctrl is JFCGridItem
                                                             select ctrl;
                        if (enumerable2.Count() > 0)
                        {
                            foreach (JFCGridItem item in enumerable2)
                            {
                                if (item.Orientation == Orientation.Horizontal)
                                {
                                    item.MouseEnterItem();
                                }
                            }
                        }
                    }
                }
                if (enumerable != null)
                {
                    bool flag2 = true;
                    if (frameworkElement is JFCBorder && (frameworkElement as JFCBorder).Orientation == Orientation.Horizontal)
                    {
                        flag2 = false;
                    }
                    if (flag2)
                    {
                        int index2 = (int)frameworkElement.GetValue(Grid.ColumnProperty);
                        JFCGridColumn jFCGridColumn = enumerable.ElementAt(index2);
                        if (jFCGridColumn.Item != null)
                        {
                            jFCGridColumn.Item.MouseEnterItem();
                        }
                    }
                }
            }
        }

        private void MyElement_MouseLeave(object sender, MouseEventArgs e)
        {
            FrameworkElement frameworkElement = sender as FrameworkElement;
            if (frameworkElement is JFCGridItem)
            {
                ((JFCGridItem)frameworkElement).MouseLeaveItem();
            }
            else
            {
                List<LinkedList<UIElement>> list = null;
                IEnumerable<JFCGridColumn> enumerable = null;
                if (frameworkElement.Parent == this.Grid10)
                {
                    list = this.TabLineH;
                    enumerable = this.ListColHeaderRow;
                }
                else if (frameworkElement.Parent == this.Grid11)
                {
                    list = this.TabLineB;
                    enumerable = this.ListColBody;
                }
                else if (frameworkElement.Parent == this.Grid12)
                {
                    list = this.TabLineF;
                    enumerable = this.ListColHeaderRowEnd;
                }
                else if (frameworkElement.Parent == this.Grid20)
                {
                    list = this.TabLineHF;
                    enumerable = this.ListColHeaderFooter;
                }
                else if (frameworkElement.Parent == this.Grid21)
                {
                    list = this.TabLineBF;
                    enumerable = this.ListColBody;
                }
                if (frameworkElement is JFCBorder)
                {
                    if ((sender as JFCBorder).Orientation == Orientation.Horizontal)
                    {
                        enumerable = null;
                    }
                    else
                    {
                        list = null;
                    }
                }
                if (list != null)
                {
                    int index = (int)frameworkElement.GetValue(Grid.RowProperty);
                    IEnumerable<UIElement> enumerable2 = from ctrl in list[index]
                                                         where ctrl is JFCGridItem
                                                         select ctrl;
                    if (enumerable2.Count() > 0)
                    {
                        foreach (JFCGridItem item in enumerable2)
                        {
                            if (item.Orientation == Orientation.Horizontal)
                            {
                                item.MouseLeaveItem();
                            }
                        }
                    }
                }
                if (enumerable != null)
                {
                    int index2 = (int)frameworkElement.GetValue(Grid.ColumnProperty);
                    JFCGridColumn jFCGridColumn = enumerable.ElementAt(index2);
                    if (jFCGridColumn.Item != null)
                    {
                        jFCGridColumn.Item.MouseLeaveItem();
                    }
                }
            }
        }

        private void MyElement_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            FrameworkElement frameworkElement = sender as FrameworkElement;
            if (frameworkElement.Tag is HierarchicalItem || frameworkElement.Tag is JFCGridCellGrouping)
            {
                frameworkElement = (FrameworkElement)frameworkElement.Tag;
            }
            int index = (int)frameworkElement.GetValue(Grid.RowProperty);
            List<LinkedList<UIElement>> list = null;
            if (frameworkElement.Parent == this.Grid10)
            {
                list = this.TabLineH;
            }
            else if (frameworkElement.Parent == this.Grid11)
            {
                list = this.TabLineB;
            }
            else if (frameworkElement.Parent == this.Grid12)
            {
                list = this.TabLineF;
            }
            else if (frameworkElement.Parent == this.Grid20)
            {
                list = this.TabLineHF;
            }
            else if (frameworkElement.Parent == this.Grid21)
            {
                list = this.TabLineBF;
            }
            if (list != null)
            {
                if (frameworkElement is JFCGridCellGrouping)
                {
                    JFCGridCellGrouping jFCGridCellGrouping = frameworkElement as JFCGridCellGrouping;
                    List<JFCExpendItem> list2 = this.FindChildren((JFCExpendItem)jFCGridCellGrouping.DataContext);
                    List<JFCExpendItem>.Enumerator enumerator;
                    if (Keyboard.IsKeyDown(Key.RightCtrl) || Keyboard.IsKeyDown(Key.LeftCtrl))
                    {
                        if (this.Parent.SelectedExpendItems.Contains((JFCExpendItem)jFCGridCellGrouping.DataContext))
                        {
                            this.Parent.SelectedExpendItems.Remove((JFCExpendItem)jFCGridCellGrouping.DataContext);
                            enumerator = list2.GetEnumerator();
                            try
                            {
                                while (enumerator.MoveNext())
                                {
                                    JFCExpendItem current = enumerator.Current;
                                    this.Parent.SelectedExpendItemsChildren.Remove(current);
                                }
                            }
                            finally
                            {
                                ((IDisposable)enumerator).Dispose();
                            }
                            this.Parent.OnSelectedItemsChanged(this.Parent, new RoutedEventArgs());
                        }
                        else if (this.Parent.SelectedExpendItems.Count > 0 && this.Parent.SelectedExpendItems.ElementAt(0).Level == ((JFCExpendItem)jFCGridCellGrouping.DataContext).Level)
                        {
                            this.Parent.SelectedExpendItems.Add((JFCExpendItem)jFCGridCellGrouping.DataContext);
                            enumerator = list2.GetEnumerator();
                            try
                            {
                                while (enumerator.MoveNext())
                                {
                                    JFCExpendItem current2 = enumerator.Current;
                                    this.Parent.SelectedExpendItemsChildren.Add(current2);
                                }
                            }
                            finally
                            {
                                ((IDisposable)enumerator).Dispose();
                            }
                            this.Parent.OnSelectedItemsChanged(this.Parent, new RoutedEventArgs());
                        }
                    }
                    else
                    {
                        this.Parent.SelectedExpendItems.RemoveWhere((JFCExpendItem row) => true);
                        this.Parent.SelectedExpendItemsChildren.Clear();
                        this.Parent.SelectedExpendItems.Add((JFCExpendItem)jFCGridCellGrouping.DataContext);
                        enumerator = list2.GetEnumerator();
                        try
                        {
                            while (enumerator.MoveNext())
                            {
                                JFCExpendItem current3 = enumerator.Current;
                                this.Parent.SelectedExpendItemsChildren.Add(current3);
                            }
                        }
                        finally
                        {
                            ((IDisposable)enumerator).Dispose();
                        }
                        this.Parent.OnSelectedItemsChanged(this.Parent, new RoutedEventArgs());
                    }
                    this.RefreshSelection();
                }
                else
                {
                    this.Parent.SelectedExpendItemsChildren.Clear();
                    IEnumerable<UIElement> source = from ctrl in list[index]
                                                    where ctrl is JFCGridItem
                                                    select ctrl;
                    if (source.Count() > 0)
                    {
                        this.item_MouseLeftButtonDown((JFCGridItem)source.First(), e);
                    }
                }
            }
        }

        private void MyElement_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            FrameworkElement frameworkElement = sender as FrameworkElement;
            if (frameworkElement.Tag is HierarchicalItem || frameworkElement.Tag is JFCGridCellGrouping)
            {
                frameworkElement = (FrameworkElement)frameworkElement.Tag;
            }
            int index = (int)frameworkElement.GetValue(Grid.RowProperty);
            List<LinkedList<UIElement>> list = null;
            if (frameworkElement.Parent == this.Grid10)
            {
                list = this.TabLineH;
            }
            else if (frameworkElement.Parent == this.Grid11)
            {
                list = this.TabLineB;
            }
            else if (frameworkElement.Parent == this.Grid12)
            {
                list = this.TabLineF;
            }
            else if (frameworkElement.Parent == this.Grid20)
            {
                list = this.TabLineHF;
            }
            else if (frameworkElement.Parent == this.Grid21)
            {
                list = this.TabLineBF;
            }
            if (list != null && !(frameworkElement is JFCGridCellGrouping))
            {
                IEnumerable<UIElement> source = from ctrl in list[index]
                                                where ctrl is JFCGridItem
                                                select ctrl;
                if (source.Count() > 0)
                {
                    this.item_MouseLeftButtonUp((JFCGridItem)source.First(), e);
                    e.Handled = true;
                }
            }
        }

        private void MyElement_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            FrameworkElement frameworkElement = sender as FrameworkElement;
            int index = (int)frameworkElement.GetValue(Grid.RowProperty);
            List<LinkedList<UIElement>> list = null;
            if (frameworkElement.Parent == this.Grid10)
            {
                list = this.TabLineH;
            }
            else if (frameworkElement.Parent == this.Grid11)
            {
                list = this.TabLineB;
            }
            else if (frameworkElement.Parent == this.Grid12)
            {
                list = this.TabLineF;
            }
            else if (frameworkElement.Parent == this.Grid20)
            {
                list = this.TabLineHF;
            }
            else if (frameworkElement.Parent == this.Grid21)
            {
                list = this.TabLineBF;
            }
            if (list != null && !(frameworkElement is JFCGridCellGrouping))
            {
                IEnumerable<UIElement> source = from ctrl in list[index]
                                                where ctrl is JFCGridItem
                                                select ctrl;
                if (source.Count() > 0)
                {
                    this.item_MouseRightButtonUp((JFCGridItem)source.First(), e);
                }
            }
        }

        private void MyElement_MouseDown(object sender, MouseButtonEventArgs e)
        {
            if (e.ClickCount == 2)
            {
                FrameworkElement frameworkElement = sender as FrameworkElement;
                JFCGridItem jFCGridItem = null;
                if (frameworkElement is JFCGridItem)
                {
                    jFCGridItem = (JFCGridItem)frameworkElement;
                }
                else
                {
                    int index = (int)frameworkElement.GetValue(Grid.RowProperty);
                    List<LinkedList<UIElement>> list = null;
                    if (frameworkElement.Parent == this.Grid10)
                    {
                        list = this.TabLineH;
                    }
                    else if (frameworkElement.Parent == this.Grid11)
                    {
                        list = this.TabLineB;
                    }
                    else if (frameworkElement.Parent == this.Grid12)
                    {
                        list = this.TabLineF;
                    }
                    else if (frameworkElement.Parent == this.Grid20)
                    {
                        list = this.TabLineHF;
                    }
                    else if (frameworkElement.Parent == this.Grid21)
                    {
                        list = this.TabLineBF;
                    }
                    if (list != null)
                    {
                        IEnumerable<UIElement> source = from ctrl in list[index]
                                                        where ctrl is JFCGridItem
                                                        select ctrl;
                        if (source.Count() > 0)
                        {
                            jFCGridItem = (JFCGridItem)source.First();
                        }
                    }
                }
                if (jFCGridItem != null)
                {
                    JFCExpendItem data = jFCGridItem.Data;
                    if (data != null && data.IsExpendable)
                    {
                        if (data.IsExpended)
                        {
                            data.IsExpended = false;
                        }
                        else
                        {
                            data.IsExpended = true;
                        }
                    }
                }
            }
        }

        private void ColumnAddGrid(JFCGridColumn Column, int Index, Grid BodyGrid, List<LinkedList<UIElement>> LstCells, IList<JFCGridColumn> LstCol)
        {
            int num = BodyGrid.RowDefinitions.Count();
            IEnumerable<UIElement> source = BodyGrid.Children.Cast<UIElement>();
            foreach (UIElement item in (from ctrl in source
                                        where (int)ctrl.GetValue(Grid.ColumnProperty) >= Index
                                        select ctrl).ToList())
            {
                if (item is JFCBorder)
                {
                    if ((item as JFCBorder).Orientation == Orientation.Vertical)
                    {
                        int num2 = (int)item.GetValue(Grid.ColumnProperty);
                        num2++;
                        item.SetValue(Grid.ColumnProperty, num2);
                    }
                }
                else if (item is JFCGridItem)
                {
                    if ((item as JFCGridItem).Orientation == Orientation.Vertical)
                    {
                        int num2 = (int)item.GetValue(Grid.ColumnProperty);
                        num2++;
                        item.SetValue(Grid.ColumnProperty, num2);
                    }
                }
                else
                {
                    int num2 = (int)item.GetValue(Grid.ColumnProperty);
                    num2++;
                    item.SetValue(Grid.ColumnProperty, num2);
                }
            }
            ColumnDefinition columnDefinition = new ColumnDefinition();
            BodyGrid.ColumnDefinitions.Insert(Index, columnDefinition);
            Binding binding = new Binding("ActualWidth");
            binding.Source = Column;
            binding.Mode = BindingMode.TwoWay;
            columnDefinition.SetBinding(ColumnDefinition.WidthProperty, binding);
            JFCBorder jFCBorder = new JFCBorder();
            jFCBorder.BorderBrush = this.Parent.VerticalBorderColor;
            jFCBorder.BorderThickness = new Thickness(0.0, 0.0, 1.0, 0.0);
            jFCBorder.Orientation = Orientation.Vertical;
            jFCBorder.SetValue(Grid.ColumnProperty, Index);
            if (num > 0)
            {
                jFCBorder.SetValue(Grid.RowSpanProperty, num);
            }
            else
            {
                jFCBorder.SetValue(Grid.RowSpanProperty, 1);
            }
            BodyGrid.Children.Add(jFCBorder);
            Column.Item.SetValue(Grid.ColumnProperty, Index);
            if (num > 0)
            {
                Column.Item.SetValue(Grid.RowSpanProperty, num);
            }
            else
            {
                Column.Item.SetValue(Grid.RowSpanProperty, 1);
            }
            Column.Item.SetValue(Panel.ZIndexProperty, -10);
            BodyGrid.Children.Add(Column.Item);
            JFCGridItem[] array = new JFCGridItem[num];
            foreach (UIElement item2 in source.Where(delegate (UIElement ctrl)
            {
                if (ctrl is JFCGridItem)
                {
                    return ((JFCGridItem)ctrl).Orientation == Orientation.Horizontal;
                }
                return false;
            }))
            {
                int num3 = (int)item2.GetValue(Grid.RowProperty);
                array[num3] = (JFCGridItem)item2;
                item2.SetValue(Grid.ColumnSpanProperty, BodyGrid.ColumnDefinitions.Count());
            }
            int num4 = BodyGrid.ColumnDefinitions.Count();
            for (int i = 0; i < num; i++)
            {
                int num5 = this.Parent.ScrollVerticalValue + i;
                if (this.Parent.dataSource == null)
                {
                    break;
                }
                if (num5 < this.Parent.dataSource.Count())
                {
                    object obj = this.Parent.dataSource[num5].Obj;
                    JFCGridItem jFCGridItem = array[i];
                    JFCExpendItem dataContext = this.Parent.dataSource[num5];
                    this.SetupCellData(BodyGrid, LstCells[i], Column, jFCGridItem, Index, i, dataContext);
                    this.RefreshData(i, num5, Column, LstCells, dataContext, LstCol);
                    this.SetupEvents(jFCGridItem);
                    foreach (UIElement item3 in LstCells[i].Where(delegate (UIElement ctrl)
                    {
                        if (ctrl is JFCBorder)
                        {
                            return ((JFCBorder)ctrl).Orientation == Orientation.Horizontal;
                        }
                        return false;
                    }))
                    {
                        if (num4 > 0)
                        {
                            item3.SetValue(Grid.ColumnSpanProperty, num4);
                        }
                        else
                        {
                            item3.SetValue(Grid.ColumnSpanProperty, 1);
                        }
                        this.SetupEvents(item3);
                    }
                }
            }
        }

        private void ColumnAddGridFooter(JFCGridColumn Column, int Index, Grid BodyGrid, List<LinkedList<UIElement>> LstCells, IList<JFCGridColumn> LstCol)
        {
            int num = BodyGrid.RowDefinitions.Count();
            IEnumerable<UIElement> source = BodyGrid.Children.Cast<UIElement>();
            foreach (UIElement item2 in (from ctrl in source
                                         where (int)ctrl.GetValue(Grid.ColumnProperty) >= Index
                                         select ctrl).ToList())
            {
                if (item2 is JFCBorder)
                {
                    if ((item2 as JFCBorder).Orientation != 0)
                    {
                        int num2 = (int)item2.GetValue(Grid.ColumnProperty);
                        num2++;
                        item2.SetValue(Grid.ColumnProperty, num2);
                    }
                }
                else if (!(item2 is JFCGridItem))
                {
                    int num2 = (int)item2.GetValue(Grid.ColumnProperty);
                    num2++;
                    item2.SetValue(Grid.ColumnProperty, num2);
                }
            }
            JFCGridItem[] array = new JFCGridItem[num];
            foreach (UIElement item3 in from ctrl in source
                                        where ctrl is JFCGridItem
                                        select ctrl)
            {
                int num3 = (int)item3.GetValue(Grid.RowProperty);
                array[num3] = (JFCGridItem)item3;
            }
            ColumnDefinition columnDefinition = new ColumnDefinition();
            BodyGrid.ColumnDefinitions.Insert(Index, columnDefinition);
            Binding binding = new Binding("ActualWidth");
            binding.Source = Column;
            binding.Mode = BindingMode.TwoWay;
            columnDefinition.SetBinding(ColumnDefinition.WidthProperty, binding);
            JFCBorder jFCBorder = new JFCBorder();
            jFCBorder.BorderBrush = this.Parent.VerticalBorderColor;
            jFCBorder.BorderThickness = new Thickness(0.0, 0.0, 1.0, 0.0);
            jFCBorder.Orientation = Orientation.Vertical;
            jFCBorder.SetValue(Grid.ColumnProperty, Index);
            if (num > 0)
            {
                jFCBorder.SetValue(Grid.RowSpanProperty, num);
            }
            else
            {
                jFCBorder.SetValue(Grid.RowSpanProperty, 1);
            }
            BodyGrid.Children.Add(jFCBorder);
            for (int i = 0; i < num; i++)
            {
                if (this.Parent.dataSourceFooter == null)
                {
                    break;
                }
                if (i < this.Parent.dataSourceFooter.Count())
                {
                    object obj = this.Parent.dataSourceFooter[i].Obj;
                    JFCGridItem item = array[i];
                    JFCExpendItem dataContext = this.Parent.dataSourceFooter[i];
                    this.SetupCellData(BodyGrid, LstCells[i], Column, item, Index, i, dataContext);
                    this.RefreshData(i, i, Column, LstCells, dataContext, LstCol);
                    foreach (UIElement item4 in LstCells[i].Where(delegate (UIElement ctrl)
                    {
                        if (ctrl is JFCBorder)
                        {
                            return ((JFCBorder)ctrl).Orientation == Orientation.Horizontal;
                        }
                        return false;
                    }))
                    {
                        int num4 = BodyGrid.ColumnDefinitions.Count();
                        if (num4 > 0)
                        {
                            item4.SetValue(Grid.ColumnSpanProperty, num4);
                        }
                        else
                        {
                            item4.SetValue(Grid.ColumnSpanProperty, 1);
                        }
                        this.SetupEvents(item4);
                    }
                    foreach (JFCGridItem item5 in (from UIElement ctrl in BodyGrid.Children
                                                   where ctrl is JFCGridItem
                                                   select ctrl).Cast<JFCGridItem>())
                    {
                        if (item5.Orientation == Orientation.Horizontal)
                        {
                            item5.SetValue(Grid.ColumnSpanProperty, BodyGrid.ColumnDefinitions.Count());
                        }
                    }
                }
            }
        }

        private void Column_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Width")
            {
                JFCGridColumn jFCGridColumn = sender as JFCGridColumn;
                if (jFCGridColumn.Width.IsAuto)
                {
                    jFCGridColumn.ActualWidth = this.CalculTailleMaxi(jFCGridColumn);
                }
            }
            else if (e.PropertyName == "TypeColumn")
            {
                JFCGridColumn jFCGridColumn2 = sender as JFCGridColumn;
                if (jFCGridColumn2.TypeColumn == JFCGridColumn.TypesColumn.Hierarchical)
                {
                    int idxCol = this.ColumnIndexOf(jFCGridColumn2);
                    if (idxCol != -1)
                    {
                        Grid grid;
                        List<LinkedList<UIElement>> list;
                        IList<JFCGridColumn> listCol;
                        if (jFCGridColumn2.Frozen == JFCGridColumn.FrozenType.Start)
                        {
                            grid = this.Grid10;
                            list = this.TabLineH;
                            listCol = this.ListColHeaderRow;
                        }
                        else if (jFCGridColumn2.Frozen == JFCGridColumn.FrozenType.End)
                        {
                            grid = this.Grid12;
                            list = this.TabLineF;
                            listCol = this.ListColHeaderRowEnd;
                        }
                        else
                        {
                            grid = this.Grid11;
                            list = this.TabLineB;
                            listCol = this.ListColBody;
                        }
                        int num = grid.RowDefinitions.Count();
                        if (num != 0)
                        {
                            JFCGridItem[] array = new JFCGridItem[num];
                            foreach (UIElement item2 in from UIElement ctrl in grid.Children
                                                        where ctrl is JFCGridItem
                                                        select ctrl)
                            {
                                int num2 = (int)item2.GetValue(Grid.RowProperty);
                                array[num2] = (JFCGridItem)item2;
                            }
                            int num3 = 0;
                            foreach (LinkedList<UIElement> item3 in list)
                            {
                                List<UIElement> list2 = new List<UIElement>();
                                foreach (UIElement item4 in from ctrl in item3
                                                            where (int)ctrl.GetValue(Grid.ColumnProperty) == idxCol
                                                            select ctrl)
                                {
                                    if (item4 is CellImg || item4 is HierarchicalItem || item4 is JFCGridCell)
                                    {
                                        list2.Add(item4);
                                    }
                                }
                                foreach (UIElement item5 in list2)
                                {
                                    this.DeleteEvents(item5, false);
                                    list[num3].Remove(item5);
                                    grid.Children.Remove(item5);
                                }
                                num3++;
                            }
                            for (int i = 0; i < num; i++)
                            {
                                int num4 = this.Parent.ScrollVerticalValue + i;
                                object obj = this.Parent.dataSource[num4].Obj;
                                JFCGridItem item = array[i];
                                JFCExpendItem dataContext = this.Parent.dataSource[num4];
                                this.SetupCellData(grid, list[i], jFCGridColumn2, item, idxCol, i, dataContext);
                                this.RefreshData(i, num4, jFCGridColumn2, list, dataContext, listCol);
                            }
                        }
                    }
                }
            }
        }

        private void ColumnAddGridHeader(Grid grid, JFCGridColumn Column, int Index)
        {
            ColumnDefinition columnDefinition = new ColumnDefinition();
            grid.ColumnDefinitions.Insert(Index, columnDefinition);
            Binding binding = new Binding("ActualWidth");
            binding.Source = Column;
            binding.Mode = BindingMode.TwoWay;
            columnDefinition.SetBinding(ColumnDefinition.WidthProperty, binding);
        }

        private void ColumnRemoveGridHeader(Grid grid, int Index)
        {
            grid.ColumnDefinitions.RemoveAt(Index);
        }

        public void RefreshHeaderColumnsWidth()
        {
            foreach (JFCGridColumn item in this.ListColHeaderRow)
            {
                GridLength gridLength = item.Width;
                if (gridLength.IsAuto)
                {
                    GridLength actualWidth = this.CalculTailleMaxi(item);
                    gridLength = item.ActualWidth;
                    if (gridLength.Value < actualWidth.Value)
                    {
                        item.ActualWidth = actualWidth;
                    }
                }
            }
        }

        public void RefreshAllColumnsWidth()
        {
            GridLength gridLength;
            GridLength actualWidth;
            foreach (JFCGridColumn item in this.ListColHeaderRow)
            {
                gridLength = item.Width;
                if (gridLength.IsAuto)
                {
                    actualWidth = this.CalculTailleMaxi(item);
                    gridLength = item.ActualWidth;
                    if (gridLength.Value < actualWidth.Value)
                    {
                        item.ActualWidth = actualWidth;
                    }
                }
            }
            if (this.Parent.IsVirtualised)
            {
                int num = 0;
                foreach (JFCGridColumn item2 in this.ListColBody)
                {
                    if (this.startIndexColView <= num && num < this.startIndexColView + this.nbColView)
                    {
                        gridLength = item2.Width;
                        if (gridLength.IsAuto)
                        {
                            actualWidth = this.CalculTailleMaxi(item2);
                            gridLength = item2.ActualWidth;
                            if (gridLength.Value < actualWidth.Value)
                            {
                                item2.ActualWidth = actualWidth;
                            }
                        }
                    }
                    num++;
                }
            }
            else
            {
                foreach (JFCGridColumn item3 in this.ListColBody)
                {
                    gridLength = item3.Width;
                    if (gridLength.IsAuto)
                    {
                        actualWidth = this.CalculTailleMaxi(item3);
                        gridLength = item3.ActualWidth;
                        if (gridLength.Value < actualWidth.Value)
                        {
                            item3.ActualWidth = actualWidth;
                        }
                    }
                }
            }
        }

        public void ColumnMove(JFCGridColumn Column, int Index)
        {
            if (Column.Frozen == JFCGridColumn.FrozenType.Start)
            {
                Grid grid;
                Grid grid2;
                IList<JFCGridColumn> list;
                if (Column.Frozen == JFCGridColumn.FrozenType.Start)
                {
                    grid = this.Grid10;
                    grid2 = this.Grid00;
                    list = this.ListColHeaderRow;
                }
                else if (Column.Frozen == JFCGridColumn.FrozenType.End)
                {
                    grid = this.Grid12;
                    grid2 = this.Grid02;
                    list = this.ListColHeaderRowEnd;
                }
                else
                {
                    grid = this.Grid11;
                    grid2 = this.Grid01;
                    list = this.ListColBody;
                }
                int indexBefore = list.IndexOf(Column);
                if (Index != indexBefore)
                {
                    list.Remove(Column);
                    List<UIElement> list2 = (from UIElement ctrl in grid.Children
                                             where (int)ctrl.GetValue(Grid.ColumnProperty) == indexBefore
                                             select ctrl).ToList();
                    IEnumerable<UIElement> source = (indexBefore >= Index) ? grid.Children.Cast<UIElement>().Where(delegate (UIElement ctrl)
                    {
                        if ((int)ctrl.GetValue(Grid.ColumnProperty) < indexBefore)
                        {
                            return (int)ctrl.GetValue(Grid.ColumnProperty) >= Index;
                        }
                        return false;
                    }) : grid.Children.Cast<UIElement>().Where(delegate (UIElement ctrl)
                    {
                        if ((int)ctrl.GetValue(Grid.ColumnProperty) > indexBefore)
                        {
                            return (int)ctrl.GetValue(Grid.ColumnProperty) <= Index;
                        }
                        return false;
                    });
                    List<UIElement> list3 = source.ToList();
                    ColumnDefinition value = grid.ColumnDefinitions[indexBefore];
                    grid.ColumnDefinitions.RemoveAt(indexBefore);
                    ColumnDefinition value2 = grid2.ColumnDefinitions[indexBefore];
                    grid2.ColumnDefinitions.RemoveAt(indexBefore);
                    bool flag = false;
                    List<UIElement>.Enumerator enumerator = list3.GetEnumerator();
                    try
                    {
                        while (enumerator.MoveNext())
                        {
                            UIElement current = enumerator.Current;
                            flag = true;
                            if (current is JFCGridItem)
                            {
                                flag = ((byte)((((JFCGridItem)current).Orientation != 0) ? 1 : 0) != 0);
                            }
                            else if (current is JFCBorder && (current as JFCBorder).Orientation == Orientation.Horizontal)
                            {
                                flag = false;
                            }
                            if (flag)
                            {
                                if (indexBefore < Index)
                                {
                                    int num = (int)current.GetValue(Grid.ColumnProperty);
                                    num--;
                                    current.SetValue(Grid.ColumnProperty, num);
                                }
                                else
                                {
                                    int num = (int)current.GetValue(Grid.ColumnProperty);
                                    num++;
                                    current.SetValue(Grid.ColumnProperty, num);
                                }
                            }
                        }
                    }
                    finally
                    {
                        ((IDisposable)enumerator).Dispose();
                    }
                    grid.ColumnDefinitions.Insert(Index, value);
                    list.Insert(Index, Column);
                    grid2.ColumnDefinitions.Insert(Index, value2);
                    enumerator = list2.GetEnumerator();
                    try
                    {
                        while (enumerator.MoveNext())
                        {
                            UIElement current2 = enumerator.Current;
                            flag = true;
                            if (current2 is JFCGridItem)
                            {
                                flag = false;
                            }
                            else if (current2 is JFCBorder && (current2 as JFCBorder).Orientation == Orientation.Horizontal)
                            {
                                flag = false;
                            }
                            if (flag)
                            {
                                current2.SetValue(Grid.ColumnProperty, Index);
                            }
                        }
                    }
                    finally
                    {
                        ((IDisposable)enumerator).Dispose();
                    }
                    int num2 = 0;
                    int num3 = 0;
                    int num4 = this.MaxLevelColumn = this.NbLevelColumn(this.Parent.ColunmDefinition, ref num2, ref num3);
                    grid2.RowDefinitions.Clear();
                    for (int i = 0; i < this.MaxLevelColumn; i++)
                    {
                        grid2.RowDefinitions.Add(new RowDefinition());
                    }
                    num2 = 0;
                    num3 = 0;
                    this.Grid00.Children.Clear();
                    this.Grid01.Children.Clear();
                    num3 = this.BuildColumnHeader(this.Parent.ColunmDefinition, ref num2, ref num3);
                }
            }
            else
            {
                int oldIndex = this.ListColBody.IndexOf(Column);
                this.ListColBody.Move(oldIndex, Index);
            }
        }

        public void ColumnRemove(JFCGridColumn Column)
        {
            Column.PropertyChanged -= this.Column_PropertyChanged;
            if (Column.Frozen == JFCGridColumn.FrozenType.Start)
            {
                int num = this.ListColHeaderRow.IndexOf(Column);
                if (num != -1)
                {
                    this.Grid00.ColumnDefinitions.RemoveAt(num);
                    this.ListColHeaderRow.Remove(Column);
                    this.ColumnRemove(num, this.Grid10, this.TabLineH);
                    goto IL_005e;
                }
                return;
            }
            goto IL_005e;
        IL_005e:
            if (Column.Frozen == JFCGridColumn.FrozenType.End)
            {
                int num2 = this.ListColHeaderRowEnd.IndexOf(Column);
                if (num2 != -1)
                {
                    this.Grid02.ColumnDefinitions.RemoveAt(num2);
                    this.ListColHeaderRowEnd.Remove(Column);
                    this.ColumnRemove(num2, this.Grid12, this.TabLineF);
                }
            }
            else if (this.ListColBody.IndexOf(Column) != -1)
            {
                this.ListColBody.Remove(Column);
            }
        }

        private void ColumnRemove(int index, Grid gridTmp, List<LinkedList<UIElement>> TabLineTmp)
        {
            this.ColumnRemove(index, gridTmp, TabLineTmp, false);
        }

        private void ColumnRemove(int index, Grid gridTmp, List<LinkedList<UIElement>> TabLineTmp, bool useLstRemoveCol)
        {
            if (index < gridTmp.ColumnDefinitions.Count())
            {
                gridTmp.ColumnDefinitions.RemoveAt(index);
                int num = gridTmp.ColumnDefinitions.Count();
                if (num == 0)
                {
                    num = 1;
                }
                foreach (UIElement item in (IEnumerable<UIElement>)gridTmp.Children.Cast<UIElement>().ToList())
                {
                    int num2 = (int)item.GetValue(Grid.ColumnProperty);
                    if (num2 == index)
                    {
                        if (item is JFCBorder)
                        {
                            JFCBorder jFCBorder = item as JFCBorder;
                            if (jFCBorder.Orientation == Orientation.Vertical)
                            {
                                gridTmp.Children.Remove(jFCBorder);
                            }
                        }
                        else if (item is JFCGridItem)
                        {
                            if ((item as JFCGridItem).Orientation == Orientation.Vertical)
                            {
                                gridTmp.Children.Remove(item);
                            }
                            else
                            {
                                item.SetValue(Grid.ColumnSpanProperty, num);
                            }
                        }
                        else if (useLstRemoveCol)
                        {
                            int num3 = (int)item.GetValue(Grid.RowProperty);
                            bool flag = false;
                            if (num3 < TabLineTmp.Count())
                            {
                                flag = TabLineTmp[num3].Remove(item);
                            }
                            this.DeleteEvents(item, false);
                            if (flag)
                            {
                                if (item is CellImg)
                                {
                                    this.LstRemoveColCellImg.Add((CellImg)item);
                                }
                                else if (item is JFCGridCell)
                                {
                                    this.LstRemoveColJFCGridCell.Add((JFCGridCell)item);
                                }
                            }
                            else
                            {
                                bool flag2 = false;
                                if (item is CellImg)
                                {
                                    if (this.LstRemoveColCellImg.Contains((CellImg)item))
                                    {
                                        flag2 = true;
                                    }
                                }
                                else if (item is JFCGridCell && this.LstRemoveColJFCGridCell.Contains((JFCGridCell)item))
                                {
                                    flag2 = true;
                                }
                                if (!flag2)
                                {
                                    gridTmp.Children.Remove(item);
                                }
                            }
                        }
                        else
                        {
                            gridTmp.Children.Remove(item);
                            int num3 = (int)item.GetValue(Grid.RowProperty);
                            if (num3 < TabLineTmp.Count())
                            {
                                TabLineTmp[num3].Remove(item);
                            }
                            this.DeleteEvents(item, false);
                            if (item is CellImg)
                            {
                                this.LstRecycleCellImg.Add((CellImg)item);
                            }
                            else if (item is JFCGridCell)
                            {
                                this.LstRecycleJFCGridCell.Add((JFCGridCell)item);
                            }
                        }
                    }
                    else if (num2 >= index)
                    {
                        int num4 = (int)item.GetValue(Grid.ColumnProperty);
                        num4--;
                        item.SetValue(Grid.ColumnProperty, num4);
                    }
                }
            }
        }

        public void ColumnRemoveAll()
        {
            List<JFCGridColumn>.Enumerator enumerator = this.ListColHeaderRow.ToList().GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    JFCGridColumn current = enumerator.Current;
                    this.ColumnRemove(current);
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            this.BuildHeader(this.Grid00);
            this.ListColBody.Clear();
            this.BuildHeader(this.Grid01);
            enumerator = this.ListColHeaderRowEnd.ToList().GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    JFCGridColumn current2 = enumerator.Current;
                    this.ColumnRemove(current2);
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            this.BuildHeader(this.Grid02);
            this.ColumnRemoveFooterAll();
        }

        public void ColumnRemoveAll(bool frozen)
        {
            if (frozen)
            {
                List<JFCGridColumn>.Enumerator enumerator = this.ListColHeaderRow.ToList().GetEnumerator();
                try
                {
                    while (enumerator.MoveNext())
                    {
                        JFCGridColumn current = enumerator.Current;
                        this.ColumnRemove(current);
                    }
                }
                finally
                {
                    ((IDisposable)enumerator).Dispose();
                }
                this.BuildHeader(this.Grid00);
                enumerator = this.ListColHeaderRowEnd.ToList().GetEnumerator();
                try
                {
                    while (enumerator.MoveNext())
                    {
                        JFCGridColumn current2 = enumerator.Current;
                        this.ColumnRemove(current2);
                    }
                }
                finally
                {
                    ((IDisposable)enumerator).Dispose();
                }
                this.BuildHeader(this.Grid02);
            }
            else
            {
                this.ListColBody.Clear();
            }
        }

        public void ColumnRemoveFooter(JFCGridColumn Column)
        {
            Column.PropertyChanged -= this.Column_PropertyChanged;
            int num = this.ListColHeaderFooter.IndexOf(Column);
            if (num != -1)
            {
                this.ListColHeaderFooter.Remove(Column);
                this.ColumnRemove(num, this.Grid20, this.TabLineHF);
            }
        }

        public void ColumnRemoveFooterAll()
        {
            foreach (JFCGridColumn item in this.ListColHeaderFooter.ToList())
            {
                this.ColumnRemoveFooter(item);
            }
            this.BuildHeader(this.Grid02);
        }

        public double ColumnWidth(JFCGridColumn.FrozenType Frozen, int Index)
        {
            GridLength gridLength;
            JFCGridColumn jFCGridColumn;
            switch (Frozen)
            {
                case JFCGridColumn.FrozenType.Start:
                    if (Index < this.ListColHeaderRow.Count())
                    {
                        gridLength = this.ListColHeaderRow[Index].ActualWidth;
                        return gridLength.Value;
                    }
                    return -1.0;
                case JFCGridColumn.FrozenType.End:
                    if (Index < this.ListColHeaderRowEnd.Count())
                    {
                        gridLength = this.ListColHeaderRowEnd[Index].ActualWidth;
                        return gridLength.Value;
                    }
                    return -1.0;
                default:
                    {
                        if (Index < this.ListColBody.Count())
                        {
                            jFCGridColumn = ((Collection<JFCGridColumn>)this.ListColBody)[Index];
                            gridLength = jFCGridColumn.Width;
                            if (gridLength.IsAuto)
                            {
                                gridLength = jFCGridColumn.ActualWidth;
                                if (!gridLength.IsAuto)
                                {
                                    gridLength = jFCGridColumn.ActualWidth;
                                    if (gridLength.Value == 0.0)
                                    {
                                        goto IL_00c0;
                                    }
                                    goto IL_00cd;
                                }
                                goto IL_00c0;
                            }
                            goto IL_00cd;
                        }
                        return -1.0;
                    }
                IL_00cd:
                    gridLength = ((Collection<JFCGridColumn>)this.ListColBody)[Index].ActualWidth;
                    return gridLength.Value;
                IL_00c0:
                    jFCGridColumn.ActualWidth = this.CalculTailleMaxi(jFCGridColumn);
                    goto IL_00cd;
            }
        }

        public int ColumnIndexOf(JFCGridColumn Column)
        {
            if (Column.Frozen == JFCGridColumn.FrozenType.Start)
            {
                return this.ListColHeaderRow.IndexOf(Column);
            }
            if (Column.Frozen == JFCGridColumn.FrozenType.End)
            {
                return this.ListColHeaderRowEnd.IndexOf(Column);
            }
            return this.ListColBody.IndexOf(Column);
        }

        public JFCGridColumn ColumnBody(int Index)
        {
            if (Index < this.ListColBody.Count)
            {
                return ((Collection<JFCGridColumn>)this.ListColBody)[Index];
            }
            return null;
        }

        public int ColumnHeaderRowFooterIndexOf(JFCGridColumn Column)
        {
            return this.ListColHeaderFooter.IndexOf(Column);
        }

        public int NbColumn(JFCGridColumn.FrozenType Frozen)
        {
            switch (Frozen)
            {
                case JFCGridColumn.FrozenType.Start:
                    return this.ListColHeaderRow.Count();
                case JFCGridColumn.FrozenType.End:
                    return this.ListColHeaderRowEnd.Count();
                default:
                    return this.ListColBody.Count();
            }
        }

        public int NbColumnHeaderRowFooter()
        {
            return this.ListColHeaderFooter.Count();
        }

        private int NbLevelColumn(JFCGridControl.ObservableCollection<JFCGridColumn> Columns, ref int Level, ref int MaxLevel)
        {
            Level++;
            if (Level > MaxLevel)
            {
                MaxLevel = Level;
            }
            foreach (JFCGridColumn Column in Columns)
            {
                if (Column.ChildrenColumns.Count() > 0)
                {
                    this.NbLevelColumn(Column.ChildrenColumns, ref Level, ref MaxLevel);
                }
            }
            Level--;
            return MaxLevel;
        }

        internal void ReBuildColumnHeader(JFCGridColumn.FrozenType ColumnFrozen)
        {
            Grid grid;
            IEnumerable<JFCGridColumn> columns;
            switch (ColumnFrozen)
            {
                case JFCGridColumn.FrozenType.Start:
                    grid = this.Grid00;
                    columns = from col in this.Parent.ColunmDefinition
                              where col.Frozen == JFCGridColumn.FrozenType.Start
                              select col;
                    break;
                case JFCGridColumn.FrozenType.End:
                    grid = this.Grid02;
                    columns = from col in this.Parent.ColunmDefinition
                              where col.Frozen == JFCGridColumn.FrozenType.End
                              select col;
                    break;
                default:
                    grid = this.Grid01;
                    columns = from col in this.Parent.ColunmDefinition
                              where col.Frozen == JFCGridColumn.FrozenType.None
                              select col;
                    break;
            }
            int num = 0;
            int num2 = 0;
            int num3 = this.MaxLevelColumn = this.NbLevelColumn(this.Parent.ColunmDefinition, ref num, ref num2);
            if (this.MaxLevelColumn != grid.RowDefinitions.Count())
            {
                grid.RowDefinitions.Clear();
                for (int i = 0; i < this.MaxLevelColumn; i++)
                {
                    grid.RowDefinitions.Add(new RowDefinition());
                }
            }
            num = 0;
            num2 = 0;
            this.LstRemoveHeader = (from UIElement item in grid.Children
                                    where item is JFCGridColumnHeader
                                    select item).ToList();
            this.LstRemoveBelowHeader = (from UIElement item in grid.Children
                                         where item is JFCGridColumnBelowHeader
                                         select item).ToList();
            this.LstRemoveSplitter = (from UIElement item in grid.Children
                                      where item is GridSplitter
                                      select item).ToList();
            num2 = this.BuildColumnHeader(columns, ref num, ref num2);
            List<UIElement>.Enumerator enumerator = this.LstRemoveHeader.GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    UIElement current = enumerator.Current;
                    grid.Children.Remove(current);
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            enumerator = this.LstRemoveBelowHeader.GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    UIElement current2 = enumerator.Current;
                    grid.Children.Remove(current2);
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            enumerator = this.LstRemoveSplitter.GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    UIElement current3 = enumerator.Current;
                    grid.Children.Remove(current3);
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
        }

        internal void BuildHeader(Grid GridHeader)
        {
            int num = 0;
            int num2 = 0;
            int num3 = this.NbLevelColumn(this.Parent.ColunmDefinition, ref num, ref num2);
            IEnumerable<JFCGridColumn> columns = null;
            if (GridHeader == this.Grid00)
            {
                columns = from col in this.Parent.ColunmDefinition
                          where col.Frozen == JFCGridColumn.FrozenType.Start
                          select col;
            }
            else if (GridHeader == this.Grid01)
            {
                columns = from col in this.Parent.ColunmDefinition
                          where col.Frozen == JFCGridColumn.FrozenType.None
                          select col;
            }
            else if (GridHeader == this.Grid02)
            {
                columns = from col in this.Parent.ColunmDefinition
                          where col.Frozen == JFCGridColumn.FrozenType.End
                          select col;
            }
            if (this.MaxLevelColumn == num3)
            {
                num = 0;
                num2 = 0;
                GridHeader.Children.Clear();
                num2 = this.BuildColumnHeader(columns, ref num, ref num2);
            }
            else
            {
                this.MaxLevelColumn = num3;
                GridHeader.RowDefinitions.Clear();
                for (int i = 0; i < this.MaxLevelColumn; i++)
                {
                    GridHeader.RowDefinitions.Add(new RowDefinition());
                }
                num = 0;
                num2 = 0;
                GridHeader.Children.Clear();
                num2 = this.BuildColumnHeader(columns, ref num, ref num2);
            }
        }

        private int BuildColumnHeader(IEnumerable<JFCGridColumn> Columns, ref int Level, ref int MaxLevel)
        {
            foreach (JFCGridColumn Column in Columns)
            {
                if (Column.ChildrenColumns.Count() > 0)
                {
                    Level++;
                    if (Level > MaxLevel)
                    {
                        MaxLevel = Level;
                    }
                    this.BuildColumnHeader((IEnumerable<JFCGridColumn>)Column.ChildrenColumns, ref Level, ref MaxLevel);
                    Level--;
                }
                this.HeaderAdd(Column, Level);
            }
            return MaxLevel;
        }

        private int CalculColSpan(JFCGridColumn col)
        {
            int num = 0;
            if (col.ChildrenColumns.Count() > 0)
            {
                {
                    foreach (JFCGridColumn childrenColumn in col.ChildrenColumns)
                    {
                        num += this.CalculColSpan(childrenColumn);
                    }
                    return num;
                }
            }
            return 1;
        }

        private void HeaderAdd(JFCGridColumn col, int Level)
        {
            int num = -1;
            int num2 = -1;
            Grid grid;
            if (col.Frozen == JFCGridColumn.FrozenType.Start)
            {
                grid = this.Grid00;
                if (col.ChildrenColumns.Count() == 0)
                {
                    num = this.ListColHeaderRow.IndexOf(col);
                    num2 = 1;
                }
                else
                {
                    num2 = this.CalculColSpan(col);
                    num = this.ListColHeaderRow.IndexOf(this.FindLastChildrenColumn(col.ChildrenColumns)) - num2 + 1;
                }
                if (num < 0)
                {
                    num = 0;
                }
                goto IL_01e3;
            }
            if (col.Frozen == JFCGridColumn.FrozenType.End)
            {
                grid = this.Grid02;
                if (col.ChildrenColumns.Count() == 0)
                {
                    num = this.ListColHeaderRowEnd.IndexOf(col);
                    num2 = 1;
                }
                else
                {
                    num2 = this.CalculColSpan(col);
                    num = this.ListColHeaderRowEnd.IndexOf(this.FindLastChildrenColumn(col.ChildrenColumns)) - num2 + 1;
                }
                if (num < 0)
                {
                    num = 0;
                }
                goto IL_01e3;
            }
            grid = this.Grid01;
            if (!this.Parent.IsVirtualised)
            {
                if (col.ChildrenColumns.Count() == 0)
                {
                    num = this.ListColBody.IndexOf(col);
                    num2 = 1;
                }
                else
                {
                    num2 = this.CalculColSpan(col);
                    num = this.ListColBody.IndexOf(this.FindLastChildrenColumn(col.ChildrenColumns)) - num2 + 1;
                }
                if (num < 0)
                {
                    num = 0;
                }
                goto IL_016d;
            }
            if (this.NbColView != 0)
            {
                if (col.ChildrenColumns.Count() == 0)
                {
                    num = this.ListColBody.IndexOf(col);
                    num2 = 1;
                }
                else
                {
                    num2 = this.CalculColSpan(col);
                    num = this.ListColBody.IndexOf(this.FindLastChildrenColumn(col.ChildrenColumns)) - num2 + 1;
                }
                goto IL_016d;
            }
            return;
        IL_01e3:
            bool flag = false;
            GridSplitter gridSplitter;
            if (this.LstRemoveSplitter.Count() > 0)
            {
                gridSplitter = (GridSplitter)this.LstRemoveSplitter.Last();
                this.LstRemoveSplitter.RemoveAt(this.LstRemoveSplitter.Count() - 1);
                flag = true;
            }
            else
            {
                gridSplitter = new GridSplitter();
            }
            col.Header.SetValue(Grid.ColumnProperty, num);
            col.Header.SetValue(Grid.RowProperty, Level);
            gridSplitter.Width = 8.0;
            gridSplitter.IsTabStop = false;
            gridSplitter.Background = Brushes.Transparent;
            gridSplitter.SetValue(Grid.ColumnProperty, num);
            gridSplitter.SetValue(Grid.RowProperty, Level);
            gridSplitter.ShowsPreview = false;
            gridSplitter.MouseDoubleClick -= this.Split_MouseDoubleClick;
            gridSplitter.MouseDoubleClick += this.Split_MouseDoubleClick;
            gridSplitter.DragCompleted -= this.Split_DragCompleted;
            gridSplitter.DragCompleted += this.Split_DragCompleted;
            gridSplitter.Tag = col;
            if (col.ChildrenColumns.Count() == 0)
            {
                col.Header.SetValue(Grid.RowSpanProperty, this.MaxLevelColumn - Level);
                gridSplitter.SetValue(Grid.RowSpanProperty, this.MaxLevelColumn - Level);
            }
            else
            {
                col.Header.SetValue(Grid.ColumnSpanProperty, num2);
                gridSplitter.SetValue(Grid.ColumnSpanProperty, num2);
                col.Header.SetValue(Grid.RowSpanProperty, 1);
                col.Header.BelowHeader.SetValue(Grid.ColumnProperty, num);
                col.Header.BelowHeader.SetValue(Grid.ColumnSpanProperty, num2);
                col.Header.BelowHeader.SetValue(Grid.RowProperty, Level + 1);
                col.Header.BelowHeader.SetValue(Grid.RowSpanProperty, this.MaxLevelColumn - Level);
                if (!this.LstRemoveBelowHeader.Contains(col.Header.BelowHeader))
                {
                    grid.Children.Insert(0, col.Header.BelowHeader);
                }
                else
                {
                    this.LstRemoveHeader.Remove(col.Header.BelowHeader);
                }
            }
            col.Header.HorizontalAlignment = HorizontalAlignment.Stretch;
            col.Header.VerticalAlignment = VerticalAlignment.Stretch;
            if (!this.LstRemoveHeader.Contains(col.Header))
            {
                grid.Children.Add(col.Header);
            }
            else
            {
                this.LstRemoveHeader.Remove(col.Header);
            }
            if (!flag)
            {
                grid.Children.Add(gridSplitter);
            }
            if (!col.IsResizable)
            {
                gridSplitter.Visibility = Visibility.Hidden;
            }
            else
            {
                gridSplitter.Visibility = Visibility.Visible;
            }
            col.Header.PreviewMouseLeftButtonDown -= this.Header_PreviewMouseLeftButtonDown;
            col.Header.PreviewMouseLeftButtonUp -= this.Header_PreviewMouseLeftButtonUp;
            col.Header.PreviewMouseMove -= this.Header_PreviewMouseMove;
            col.Header.PreviewMouseLeftButtonDown += this.Header_PreviewMouseLeftButtonDown;
            col.Header.PreviewMouseLeftButtonUp += this.Header_PreviewMouseLeftButtonUp;
            col.Header.PreviewMouseMove += this.Header_PreviewMouseMove;
            return;
        IL_016d:
            if (this.NbColView == 0 && this.Parent.IsVirtualised)
            {
                return;
            }
            if (num + num2 - 1 >= this.StartIndexColView && num <= this.StartIndexColView + this.NbColView - 1)
            {
                if (num < this.StartIndexColView)
                {
                    num2 -= this.StartIndexColView - num;
                    num = 0;
                }
                else
                {
                    num -= this.StartIndexColView;
                }
                if (this.NbColView < num + num2)
                {
                    num2 = this.NbColView - num;
                }
                if (num >= 0 && num2 >= 0)
                {
                    goto IL_01e3;
                }
            }
        }

        public List<JFCGridColumn> FindChildrenColumn(JFCGridControl.ObservableCollection<JFCGridColumn> Columns)
        {
            List<JFCGridColumn> list = new List<JFCGridColumn>();
            foreach (JFCGridColumn Column in Columns)
            {
                if (Column.ChildrenColumns.Count() > 0)
                {
                    list.AddRange(this.FindChildrenColumn(Column.ChildrenColumns));
                }
                else
                {
                    list.Add(Column);
                }
            }
            return list;
        }

        public List<JFCGridColumn> FindChildrenColumn(JFCGridColumn Column)
        {
            List<JFCGridColumn> list = new List<JFCGridColumn>();
            if (Column.ChildrenColumns.Count() > 0)
            {
                list.AddRange(this.FindChildrenColumn(Column.ChildrenColumns));
            }
            return list;
        }

        public void BuildParentColumn(JFCGridColumn Column)
        {
            new List<JFCGridColumn>();
            foreach (JFCGridColumn childrenColumn in Column.ChildrenColumns)
            {
                if (childrenColumn.ChildrenColumns.Count() > 0)
                {
                    this.FindChildrenColumn(childrenColumn.ChildrenColumns);
                }
            }
        }

        public JFCGridColumn FindLastChildrenColumn(JFCGridControl.ObservableCollection<JFCGridColumn> Columns)
        {
            JFCGridColumn jFCGridColumn = Columns.Last();
            if (jFCGridColumn.ChildrenColumns.Count() == 0)
            {
                return jFCGridColumn;
            }
            return this.FindLastChildrenColumn(jFCGridColumn.ChildrenColumns);
        }

        public bool ContainColumn(IEnumerable<JFCGridColumn> Columns, JFCGridColumn Column)
        {
            foreach (JFCGridColumn Column2 in Columns)
            {
                if (Column2 == Column)
                {
                    return true;
                }
                if (Column2.ChildrenColumns.Count() > 0)
                {
                    return this.ContainColumn(Column2.ChildrenColumns, Column);
                }
            }
            return false;
        }

        public UIElement cloneElement(UIElement orig)
        {
            if (orig == null)
            {
                return null;
            }
            return (UIElement)XamlReader.Load(XmlReader.Create(new StringReader(XamlWriter.Save(orig)), new XmlReaderSettings()));
        }

        internal GridLength CalculTailleHeaderMaxi(JFCGridColumn col)
        {
            double num = 0.0;
            Size availableSize = new Size(1000.0, 1000.0);
            JFCGridColumnHeader jFCGridColumnHeader = new JFCGridColumnHeader();
            object content = col.Header.Content;
            col.Header.ContentChanged -= this.Parent.Header_ContentChanged;
            col.Header.Content = null;
            jFCGridColumnHeader.Content = content;
            jFCGridColumnHeader.Template = col.Header.Template;
            jFCGridColumnHeader.Measure(availableSize);
            num = Math.Ceiling(jFCGridColumnHeader.DesiredSize.Width);
            jFCGridColumnHeader.Content = null;
            col.Header.Content = content;
            col.Header.ContentChanged += this.Parent.Header_ContentChanged;
            return new GridLength(num);
        }

        private GridLength CalculTailleMaxi(JFCGridColumn col)
        {
            JFCExpendItem[] source = this.Parent.dataSource;
            JFCGridColumn jFCGridColumn = col;
            bool flag = false;
            if (this.Parent.dataSource == null)
            {
                if (this.Parent.dataSourceFooter == null)
                {
                    flag = true;
                }
                else if (this.Parent.dataSourceFooter.Count() == 0)
                {
                    flag = true;
                }
                else
                {
                    source = this.Parent.dataSourceFooter;
                }
            }
            else if (this.Parent.dataSource.Count() == 0)
            {
                if (this.Parent.dataSourceFooter == null)
                {
                    flag = true;
                }
                else if (this.Parent.dataSourceFooter.Count() == 0)
                {
                    flag = true;
                }
                else
                {
                    source = this.Parent.dataSourceFooter;
                }
            }
            double num = 0.0;
            double num2 = 0.0;
            Size availableSize = new Size(1000.0, 1000.0);
            if (!flag)
            {
                CellImg cellImg = new CellImg();
                JFCGridCell jFCGridCell = new JFCGridCell();
                HierarchicalItem hierarchicalItem = new HierarchicalItem();
                JFCGridCellGrouping jFCGridCellGrouping = new JFCGridCellGrouping();
                MarginConverter marginConverter = new MarginConverter();
                int num3 = 0;
                Thickness thickness;
                foreach (JFCExpendItem item in source.Take(100))
                {
                    if (col.TypeColumn == JFCGridColumn.TypesColumn.Hierarchical)
                    {
                        if (col.CellTemplate == null)
                        {
                            hierarchicalItem.DataContext = item;
                            object obj = default(object);
                            if (item != null && ((JFCGridColumnBase)col).GetCellValue(item.Obj, out obj))
                            {
                                if (obj != null)
                                {
                                    cellImg.Text = obj.ToString();
                                }
                                else
                                {
                                    cellImg.Text = "";
                                }
                            }
                            else if (col.BindingPath != "" && col.BindingPath != null)
                            {
                                PropertyInfo property = item.Obj.GetType().GetProperty(col.BindingPath);
                                if (property != (PropertyInfo)null)
                                {
                                    cellImg.Text = property.GetValue(item.Obj, null).ToString();
                                }
                                else
                                {
                                    Binding binding = new Binding(col.BindingPath);
                                    binding.Source = item.Obj;
                                    cellImg.SetBinding(CellImg.TextProperty, binding);
                                }
                            }
                            else
                            {
                                cellImg.Text = item.Obj.GetType().FullName;
                            }
                            this.ApplyStyleCell(cellImg, col, ApplyMode.CalculSize);
                            hierarchicalItem.Component = cellImg;
                            hierarchicalItem.Measure(availableSize);
                            num = hierarchicalItem.DesiredSize.Width;
                            double num4 = num;
                            thickness = (Thickness)marginConverter.Convert(item.Level, null, null, null);
                            num = num4 + thickness.Left;
                            if (num2 < num)
                            {
                                num2 = num;
                            }
                            hierarchicalItem.Component = null;
                        }
                        else
                        {
                            hierarchicalItem = new HierarchicalItem();
                            jFCGridCell = new JFCGridCell();
                            jFCGridCell.Template = col.CellTemplate;
                            hierarchicalItem.DataContext = item;
                            object dataContext = default(object);
                            if (item != null && ((JFCGridColumnBase)col).GetCellValue(item.Obj, out dataContext))
                            {
                                jFCGridCell.DataContext = dataContext;
                            }
                            else if (col.BindingPath != "" && col.BindingPath != null && item != null)
                            {
                                PropertyInfo property2 = item.Obj.GetType().GetProperty(col.BindingPath);
                                if (property2 != (PropertyInfo)null)
                                {
                                    jFCGridCell.DataContext = property2.GetValue(item.Obj, null);
                                }
                                else
                                {
                                    Binding binding2 = new Binding(col.BindingPath);
                                    binding2.Source = item.Obj;
                                    jFCGridCell.SetBinding(FrameworkElement.DataContextProperty, binding2);
                                }
                            }
                            else if (item == null)
                            {
                                jFCGridCell.DataContext = null;
                            }
                            else
                            {
                                jFCGridCell.DataContext = item.Obj;
                            }
                            hierarchicalItem.Component = jFCGridCell;
                            hierarchicalItem.Measure(availableSize);
                            num = hierarchicalItem.DesiredSize.Width;
                            double num5 = num;
                            thickness = (Thickness)marginConverter.Convert(item.Level, null, null, null);
                            num = num5 + thickness.Left;
                            if (num2 < num)
                            {
                                num2 = num;
                            }
                            hierarchicalItem.Component = null;
                        }
                    }
                    else if (col.TypeColumn == JFCGridColumn.TypesColumn.GroupingNoRow || col.TypeColumn == JFCGridColumn.TypesColumn.GroupingWithRow || col.TypeColumn == JFCGridColumn.TypesColumn.GroupingWithRowWhenNoChildren)
                    {
                        JFCExpendItem jFCExpendItem = item;
                        if (jFCExpendItem.StartChildViewIndex == num3 || this.Parent.ScrollVerticalValue == num3)
                        {
                            jFCGridCellGrouping = new JFCGridCellGrouping();
                            if (col.CellTemplate == null)
                            {
                                if (col.LevelGrouping >= 0)
                                {
                                    while (jFCExpendItem != null && jFCExpendItem.Level != col.LevelGrouping)
                                    {
                                        jFCExpendItem = jFCExpendItem.Parent;
                                    }
                                }
                                else
                                {
                                    jFCExpendItem = null;
                                }
                                if (jFCExpendItem != null)
                                {
                                    jFCGridCellGrouping.DataContext = jFCExpendItem;
                                    object obj2 = default(object);
                                    if (item != null && ((JFCGridColumnBase)col).GetCellValue(jFCExpendItem.Obj, out obj2))
                                    {
                                        if (obj2 != null)
                                        {
                                            cellImg.Text = obj2.ToString();
                                        }
                                        else
                                        {
                                            cellImg.Text = "";
                                        }
                                    }
                                    else if (col.BindingPath != "" && col.BindingPath != null)
                                    {
                                        PropertyInfo property3 = jFCExpendItem.Obj.GetType().GetProperty(col.BindingPath);
                                        if (property3 != (PropertyInfo)null)
                                        {
                                            cellImg.Text = property3.GetValue(jFCExpendItem.Obj, null).ToString();
                                        }
                                        else
                                        {
                                            Binding binding3 = new Binding(col.BindingPath);
                                            binding3.Source = jFCExpendItem.Obj;
                                            cellImg.SetBinding(CellImg.TextProperty, binding3);
                                        }
                                    }
                                    else
                                    {
                                        cellImg.Text = jFCExpendItem.Obj.GetType().FullName;
                                    }
                                    this.ApplyStyleCell(cellImg, col, ApplyMode.CalculSize);
                                    jFCGridCellGrouping.Component = cellImg;
                                    jFCGridCellGrouping.Measure(availableSize);
                                    num = jFCGridCellGrouping.DesiredSize.Width;
                                    if (num2 < num)
                                    {
                                        num2 = num;
                                    }
                                    jFCGridCellGrouping.Component = null;
                                }
                            }
                            else
                            {
                                jFCGridCell.Template = null;
                                jFCGridCellGrouping.Component = null;
                                jFCGridCell.Template = col.CellTemplate;
                                if (col.LevelGrouping >= 0)
                                {
                                    while (jFCExpendItem != null && jFCExpendItem.Level != col.LevelGrouping)
                                    {
                                        jFCExpendItem = jFCExpendItem.Parent;
                                    }
                                }
                                else
                                {
                                    jFCExpendItem = null;
                                }
                                if (jFCExpendItem != null)
                                {
                                    jFCGridCellGrouping.DataContext = jFCExpendItem;
                                    object dataContext2 = default(object);
                                    if (item != null && ((JFCGridColumnBase)col).GetCellValue(jFCExpendItem.Obj, out dataContext2))
                                    {
                                        jFCGridCell.DataContext = dataContext2;
                                    }
                                    else if (col.BindingPath != "" && col.BindingPath != null && jFCExpendItem != null)
                                    {
                                        PropertyInfo property4 = jFCExpendItem.Obj.GetType().GetProperty(col.BindingPath);
                                        if (property4 != (PropertyInfo)null)
                                        {
                                            jFCGridCell.DataContext = property4.GetValue(jFCExpendItem.Obj, null);
                                        }
                                        else
                                        {
                                            Binding binding4 = new Binding(col.BindingPath);
                                            binding4.Source = jFCExpendItem.Obj;
                                            jFCGridCell.SetBinding(FrameworkElement.DataContextProperty, binding4);
                                        }
                                    }
                                    else if (item == null)
                                    {
                                        jFCGridCell.DataContext = null;
                                    }
                                    else
                                    {
                                        jFCGridCell.DataContext = jFCExpendItem.Obj;
                                    }
                                    jFCGridCellGrouping.Component = jFCGridCell;
                                    jFCGridCellGrouping.Measure(availableSize);
                                    num = jFCGridCellGrouping.DesiredSize.Width;
                                    if (num2 < num)
                                    {
                                        num2 = num;
                                    }
                                    jFCGridCellGrouping.Component = null;
                                }
                            }
                        }
                    }
                    else if (col.CellTemplate == null)
                    {
                        this.ApplyStyleCell(cellImg, col, ApplyMode.CalculSize);
                        object obj3 = default(object);
                        if (item != null && ((JFCGridColumnBase)col).GetCellValue(item.Obj, out obj3))
                        {
                            if (obj3 != null)
                            {
                                cellImg.Text = obj3.ToString();
                            }
                            else
                            {
                                cellImg.Text = "";
                            }
                        }
                        else if (col.BindingPath != "" && col.BindingPath != null)
                        {
                            PropertyInfo property5 = item.Obj.GetType().GetProperty(col.BindingPath);
                            if (property5 != (PropertyInfo)null)
                            {
                                cellImg.Text = property5.GetValue(item.Obj, null).ToString();
                            }
                            else
                            {
                                Binding binding5 = new Binding(col.BindingPath);
                                binding5.Source = item.Obj;
                                cellImg.SetBinding(CellImg.TextProperty, binding5);
                            }
                        }
                        else
                        {
                            cellImg.Text = item.Obj.GetType().FullName;
                        }
                        cellImg.Measure(availableSize);
                        num = cellImg.DesiredSize.Width;
                        if (num2 < num)
                        {
                            num2 = num;
                        }
                    }
                    else
                    {
                        jFCGridCell.DataContext = null;
                        jFCGridCell.Template = null;
                        jFCGridCell.Template = col.CellTemplate;
                        object dataContext3 = default(object);
                        if (item != null && ((JFCGridColumnBase)col).GetCellValue(item.Obj, out dataContext3))
                        {
                            jFCGridCell.DataContext = dataContext3;
                        }
                        else if (col.BindingPath != "" && col.BindingPath != null)
                        {
                            PropertyInfo property6 = item.Obj.GetType().GetProperty(col.BindingPath);
                            if (property6 != (PropertyInfo)null)
                            {
                                jFCGridCell.DataContext = property6.GetValue(item.Obj, null);
                            }
                            else
                            {
                                Binding binding6 = new Binding(col.BindingPath);
                                binding6.Source = item.Obj;
                                jFCGridCell.SetBinding(FrameworkElement.DataContextProperty, binding6);
                            }
                        }
                        else
                        {
                            jFCGridCell.DataContext = item.Obj;
                        }
                        jFCGridCell.Measure(availableSize);
                        num = jFCGridCell.DesiredSize.Width;
                        if (num2 < num)
                        {
                            num2 = num;
                        }
                    }
                    num3++;
                }
                double num6 = num2;
                thickness = col.CellPadding;
                double left = thickness.Left;
                thickness = col.CellPadding;
                num2 = num6 + (left + thickness.Right);
            }
            while (col != null)
            {
                num = this.CalculTailleHeaderMaxi(col).Value;
                if (num2 < num)
                {
                    num2 = num;
                }
                col = ((col.Parent == null) ? null : ((col.Parent.ChildrenColumns.Count() != 1) ? null : col.Parent));
            }
            if (jFCGridColumn != null)
            {
                if (num2 <= (double)jFCGridColumn.WidthAutoMax)
                {
                    return new GridLength(Math.Ceiling(num2));
                }
                return new GridLength((double)jFCGridColumn.WidthAutoMax);
            }
            return new GridLength(Math.Ceiling(num2));
        }

        internal void ResetColumnsWidth()
        {
            List<JFCGridColumn>.Enumerator enumerator = this.ListColHeaderRow.GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    JFCGridColumn current = enumerator.Current;
                    if (current.Width == GridLength.Auto)
                    {
                        current.actualWidth = new GridLength(0.0);
                    }
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            enumerator = this.ListColHeaderRowEnd.GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    JFCGridColumn current2 = enumerator.Current;
                    if (current2.Width == GridLength.Auto)
                    {
                        current2.actualWidth = new GridLength(0.0);
                    }
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            foreach (JFCGridColumn item in this.ListColBody)
            {
                if (item.Width == GridLength.Auto)
                {
                    item.actualWidth = new GridLength(0.0);
                }
            }
        }

        private void Split_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            JFCGridColumn jFCGridColumn = (sender as GridSplitter).Tag as JFCGridColumn;
            jFCGridColumn.ActualWidth = this.CalculTailleMaxi(jFCGridColumn);
        }

        private void Split_DragCompleted(object sender, DragCompletedEventArgs e)
        {
            JFCGridColumn obj = (sender as GridSplitter).Tag as JFCGridColumn;
            obj.Width = obj.ActualWidth;
            if (this.Parent.IsVirtualised)
            {
                this.NbColView = this.Parent.NbColumnAffichable();
            }
        }

        private void Header_PreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (this.Parent.ColumnMovable)
            {
                JFCGridColumnHeader jFCGridColumnHeader = sender as JFCGridColumnHeader;
                if (jFCGridColumnHeader.Column.IsMovable)
                {
                    VisualBrush fill = new VisualBrush(jFCGridColumnHeader);
                    this.HeaderMove.Fill = fill;
                    this.PtHeaderMove = e.GetPosition(jFCGridColumnHeader);
                    this.HeaderMove.Width = jFCGridColumnHeader.ActualWidth;
                    Grid grid = null;
                    IEnumerable<JFCGridColumn> source = from c in this.Parent.ColunmDefinition
                                                        where c.Frozen == JFCGridColumn.FrozenType.Start
                                                        select c;
                    IEnumerable<JFCGridColumn> source2 = from c in this.Parent.ColunmDefinition
                                                         where c.Frozen == JFCGridColumn.FrozenType.End
                                                         select c;
                    List<JFCGridColumn> columns = source.ToList();
                    source2.ToList();
                    grid = ((!this.ContainColumn(columns, jFCGridColumnHeader.Column)) ? ((!this.ContainColumn(columns, jFCGridColumnHeader.Column)) ? this.Grid01 : this.Grid02) : this.Grid00);
                    int num = (int)jFCGridColumnHeader.GetValue(Grid.RowProperty);
                    int num2 = (int)jFCGridColumnHeader.GetValue(Grid.RowSpanProperty);
                    int num3 = 0;
                    int num4 = 0;
                    num3 = ((jFCGridColumnHeader.Column.Parent != null) ? ((int)jFCGridColumnHeader.Column.Parent.Header.GetValue(Grid.ColumnProperty)) : 0);
                    int num5 = (int)jFCGridColumnHeader.GetValue(Grid.ColumnProperty);
                    num4 = ((jFCGridColumnHeader.Column.Parent != null) ? this.FindChildrenColumn(jFCGridColumnHeader.Column.Parent.ChildrenColumns).Count() : grid.ColumnDefinitions.Count());
                    this.HeaderMove.SetValue(Grid.RowProperty, num);
                    this.HeaderMove.SetValue(Grid.RowSpanProperty, num2);
                    this.HeaderMove.SetValue(Grid.ColumnProperty, num3);
                    this.HeaderMove.SetValue(Grid.ColumnSpanProperty, num4);
                    this.HeaderMove.HorizontalAlignment = HorizontalAlignment.Left;
                    if (grid != null)
                    {
                        this.HeaderMove.SetValue(Panel.ZIndexProperty, 1000);
                        this.HeaderMove.Opacity = 0.75;
                        if (this.HeaderMove.Parent != null)
                        {
                            Grid grid2 = this.HeaderMove.Parent as Grid;
                            if (grid2 != null)
                            {
                                grid2.Children.Remove(this.HeaderMove);
                            }
                        }
                        grid.Children.Add(this.HeaderMove);
                    }
                }
            }
        }

        private void Header_PreviewMouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            if (this.Parent.ColumnMovable && this.HeaderMove != null && this.HeaderMove.Parent != null)
            {
                JFCGridColumnHeader jFCGridColumnHeader = sender as JFCGridColumnHeader;
                Grid grid = null;
                IEnumerable<JFCGridColumn> source = from c in this.Parent.ColunmDefinition
                                                    where c.Frozen == JFCGridColumn.FrozenType.Start
                                                    select c;
                IEnumerable<JFCGridColumn> source2 = from c in this.Parent.ColunmDefinition
                                                     where c.Frozen == JFCGridColumn.FrozenType.End
                                                     select c;
                List<JFCGridColumn> columns = source.ToList();
                List<JFCGridColumn> columns2 = source2.ToList();
                JFCGridColumn.FrozenType frozenType = JFCGridColumn.FrozenType.None;
                if (this.ContainColumn(columns, jFCGridColumnHeader.Column))
                {
                    grid = this.Grid00;
                    frozenType = JFCGridColumn.FrozenType.Start;
                }
                if (this.ContainColumn(columns2, jFCGridColumnHeader.Column))
                {
                    grid = this.Grid02;
                    frozenType = JFCGridColumn.FrozenType.End;
                }
                else
                {
                    grid = this.Grid01;
                    frozenType = JFCGridColumn.FrozenType.None;
                }
                Point position = e.GetPosition(grid);
                GridLength gridLength;
                if (jFCGridColumnHeader.Column.Parent != null)
                {
                    int num = (int)this.HeaderMove.GetValue(Grid.ColumnProperty);
                    double num2 = 0.0;
                    for (int i = 0; i < num; i++)
                    {
                        double num3 = num2;
                        gridLength = grid.ColumnDefinitions[i].Width;
                        num2 = num3 + gridLength.Value;
                    }
                    position.X -= num2;
                }
                if (grid != null)
                {
                    grid.Children.Remove(this.HeaderMove);
                }
                int num4 = 0;
                int num5 = 0;
                double num6 = 0.0;
                int num7 = 0;
                if (jFCGridColumnHeader.Column.Parent == null)
                {
                    JFCGridControl.ObservableCollection<JFCGridColumn> colunmDefinition = this.Parent.ColunmDefinition;
                    num7 = colunmDefinition.IndexOf(jFCGridColumnHeader.Column);
                    if (num7 >= 0)
                    {
                        foreach (JFCGridColumn item in colunmDefinition)
                        {
                            if (item.Frozen == jFCGridColumnHeader.Column.Frozen)
                            {
                                if (frozenType == JFCGridColumn.FrozenType.None && num5 < this.startIndexColView)
                                {
                                    int num8 = this.CalculColSpan(item);
                                    if (num5 + num8 - 1 < this.startIndexColView)
                                    {
                                        num5 += num8;
                                        num4++;
                                    }
                                    else
                                    {
                                        List<JFCGridColumn> list = this.FindChildrenColumn(item.ChildrenColumns);
                                        for (int j = 0; j < num8; j++)
                                        {
                                            if (this.startIndexColView <= num5)
                                            {
                                                double num9 = num6;
                                                gridLength = list[j].ActualWidth;
                                                num6 = num9 + gridLength.Value;
                                            }
                                            num5++;
                                        }
                                        num4++;
                                    }
                                    continue;
                                }
                                if (num4 < num7)
                                {
                                    if (num6 < position.X)
                                    {
                                        double x = position.X;
                                        double num10 = num6;
                                        gridLength = item.ActualWidth;
                                        if (x < num10 + gridLength.Value)
                                        {
                                            double x2 = position.X;
                                            double num11 = num6;
                                            gridLength = item.ActualWidth;
                                            if (x2 < num11 + gridLength.Value / 2.0)
                                            {
                                                colunmDefinition.Move(num7, num4);
                                            }
                                            else
                                            {
                                                colunmDefinition.Move(num7, num4 + 1);
                                            }
                                            break;
                                        }
                                    }
                                }
                                else if (num7 < num4 && num6 < position.X)
                                {
                                    double x3 = position.X;
                                    double num12 = num6;
                                    gridLength = item.ActualWidth;
                                    if (x3 < num12 + gridLength.Value)
                                    {
                                        double x4 = position.X;
                                        double num13 = num6;
                                        gridLength = item.ActualWidth;
                                        if (x4 < num13 + gridLength.Value / 2.0)
                                        {
                                            colunmDefinition.Move(num7, num4 - 1);
                                        }
                                        else
                                        {
                                            colunmDefinition.Move(num7, num4);
                                        }
                                        break;
                                    }
                                }
                                double num14 = num6;
                                gridLength = item.ActualWidth;
                                num6 = num14 + gridLength.Value;
                            }
                            num4++;
                            if (item.Frozen == JFCGridColumn.FrozenType.None)
                            {
                                num5++;
                            }
                        }
                    }
                }
            }
        }

        private void Header_PreviewMouseMove(object sender, MouseEventArgs e)
        {
            if (this.HeaderMove != null && this.HeaderMove.Parent != null && e.LeftButton == MouseButtonState.Pressed)
            {
                JFCGridColumnHeader jFCGridColumnHeader = sender as JFCGridColumnHeader;
                JFCGridColumn column = jFCGridColumnHeader.Column;
                Grid grid = null;
                Grid grid2 = null;
                IEnumerable<JFCGridColumn> source = from c in this.Parent.ColunmDefinition
                                                    where c.Frozen == JFCGridColumn.FrozenType.Start
                                                    select c;
                IEnumerable<JFCGridColumn> source2 = from c in this.Parent.ColunmDefinition
                                                     where c.Frozen == JFCGridColumn.FrozenType.End
                                                     select c;
                List<JFCGridColumn> columns = source.ToList();
                List<JFCGridColumn> columns2 = source2.ToList();
                if (this.ContainColumn(columns, jFCGridColumnHeader.Column))
                {
                    grid = this.Grid00;
                    grid2 = this.Grid10;
                }
                else if (this.ContainColumn(columns2, jFCGridColumnHeader.Column))
                {
                    grid = this.Grid02;
                    grid2 = this.Grid12;
                }
                else
                {
                    grid = this.Grid01;
                    grid2 = this.Grid11;
                }
                if (grid != null)
                {
                    Point position = e.GetPosition(grid2);
                    if (column.Parent != null)
                    {
                        int num = (int)this.HeaderMove.GetValue(Grid.ColumnProperty);
                        double num2 = 0.0;
                        for (int i = 0; i < num; i++)
                        {
                            num2 += grid.ColumnDefinitions[i].Width.Value;
                        }
                        position.X -= num2;
                    }
                    this.HeaderMove.Margin = new Thickness(position.X - this.PtHeaderMove.X, 0.0, 0.0, 0.0);
                }
            }
            else
            {
                Grid grid3 = this.HeaderMove.Parent as Grid;
                if (grid3 != null)
                {
                    grid3.Children.Remove(this.HeaderMove);
                }
            }
        }

        private void ClearParentLstRemoveCol()
        {
            foreach (CellImg item in this.LstRemoveColCellImg)
            {
                if (item.Parent is Grid)
                {
                    ((Grid)item.Parent).Children.Remove(item);
                    item.Column = null;
                    this.DeleteEvents(item, false);
                }
            }
            foreach (JFCGridCell item2 in this.LstRemoveColJFCGridCell)
            {
                if (item2.Parent is Grid)
                {
                    ((Grid)item2.Parent).Children.Remove(item2);
                    this.DeleteEvents(item2, false);
                }
            }
        }

        private void ClearLstRemoveCol()
        {
            this.ClearParentLstRemoveCol();
            this.LstRecycleCellImg.AddRange(this.LstRemoveColCellImg);
            this.LstRemoveColCellImg.Clear();
            this.LstRecycleJFCGridCell.AddRange(this.LstRemoveColJFCGridCell);
            this.LstRemoveColJFCGridCell.Clear();
        }

        public void RowAdd(bool UpdateData)
        {
            int index = this.Grid11.RowDefinitions.Count();
            if (this.RowAdd(index) && UpdateData)
            {
                this.RowRefreshData(index);
            }
        }

        public bool RowAdd(int Index)
        {
            if (this.Parent.dataSource == null)
            {
                return false;
            }
            int num = this.Parent.ScrollVerticalValue + Index;
            if (num >= this.Parent.dataSource.Count())
            {
                return false;
            }
            JFCExpendItem dataContext = this.Parent.dataSource[num];
            JFCGridItem jFCGridItem = new JFCGridItem(null);
            JFCGridItem jFCGridItem2 = new JFCGridItem(null);
            JFCGridItem jFCGridItem3 = new JFCGridItem(null);
            jFCGridItem.Peer.Add(jFCGridItem2);
            jFCGridItem.Peer.Add(jFCGridItem3);
            jFCGridItem2.Peer.Add(jFCGridItem);
            jFCGridItem2.Peer.Add(jFCGridItem3);
            jFCGridItem3.Peer.Add(jFCGridItem);
            jFCGridItem3.Peer.Add(jFCGridItem2);
            jFCGridItem.Orientation = Orientation.Horizontal;
            jFCGridItem2.Orientation = Orientation.Horizontal;
            jFCGridItem3.Orientation = Orientation.Horizontal;
            Panel.SetZIndex(jFCGridItem, -1);
            Panel.SetZIndex(jFCGridItem2, -1);
            Panel.SetZIndex(jFCGridItem3, -1);
            this.RowAddGrid(this.Grid10, jFCGridItem, Index, num, this.TabLineH, dataContext, this.ListColHeaderRow);
            this.RowAddGridVirtual(this.Grid11, jFCGridItem2, Index, num, this.TabLineB, dataContext, this.ListColBody);
            this.RowAddGrid(this.Grid12, jFCGridItem3, Index, num, this.TabLineF, dataContext, this.ListColHeaderRowEnd);
            return true;
        }

        public void RowFooterAdd(bool UpdateData)
        {
            int index = this.Grid21.RowDefinitions.Count();
            this.RowFooterAdd(index);
            if (UpdateData)
            {
                this.RowFooterRefreshData(index);
            }
        }

        public void RowFooterAdd(int Index)
        {
            if (this.Parent.dataSourceFooter != null && Index < this.Parent.dataSourceFooter.Count())
            {
                JFCExpendItem dataContext = this.Parent.dataSourceFooter[Index];
                JFCGridItem jFCGridItem = new JFCGridItem(null);
                JFCGridItem jFCGridItem2 = new JFCGridItem(null);
                jFCGridItem.Peer.Add(jFCGridItem2);
                jFCGridItem2.Peer.Add(jFCGridItem);
                jFCGridItem.Orientation = Orientation.Horizontal;
                jFCGridItem2.Orientation = Orientation.Horizontal;
                Panel.SetZIndex(jFCGridItem, -1);
                Panel.SetZIndex(jFCGridItem2, -1);
                this.RowAddGrid(this.Grid20, jFCGridItem, Index, Index, this.TabLineHF, dataContext, this.ListColHeaderFooter);
                this.RowAddGridVirtual(this.Grid21, jFCGridItem2, Index, Index, this.TabLineBF, dataContext, this.ListColBody);
            }
        }

        private void RowAddGrid(Grid grid, JFCGridItem item, int Index, int IndexData, List<LinkedList<UIElement>> TabLineGrid, JFCExpendItem DataContext, List<JFCGridColumn> ListCol)
        {
            RowDefinition rowDefinition = new RowDefinition();
            rowDefinition.Height = new GridLength((double)this.Parent.RowHeight);
            grid.RowDefinitions.Insert(Index, rowDefinition);
            int num = grid.RowDefinitions.Count();
            LinkedList<UIElement> linkedList = new LinkedList<UIElement>();
            linkedList.AddLast(item);
            foreach (UIElement item2 in grid.Children.Cast<UIElement>().Where(delegate (UIElement ctrl)
            {
                if (!(ctrl is JFCBorder))
                {
                    return ctrl is JFCGridItem;
                }
                return true;
            }))
            {
                if (item2 is JFCBorder)
                {
                    if ((item2 as JFCBorder).Orientation == Orientation.Vertical)
                    {
                        item2.SetValue(Grid.RowProperty, 0);
                        item2.SetValue(Grid.RowSpanProperty, num);
                    }
                }
                else if (item2 is JFCGridItem && (item2 as JFCGridItem).Orientation == Orientation.Vertical)
                {
                    item2.SetValue(Grid.RowProperty, 0);
                    item2.SetValue(Grid.RowSpanProperty, num);
                }
            }
            for (int i = Index; i < TabLineGrid.Count(); i++)
            {
                foreach (UIElement item3 in TabLineGrid[i])
                {
                    if (item3 is JFCBorder)
                    {
                        if ((item3 as JFCBorder).Orientation != Orientation.Vertical)
                        {
                            item3.SetValue(Grid.RowProperty, i + 1);
                        }
                    }
                    else if (item3 is JFCGridItem)
                    {
                        if ((item3 as JFCGridItem).Orientation != Orientation.Vertical)
                        {
                            item3.SetValue(Grid.RowProperty, i + 1);
                        }
                    }
                    else
                    {
                        i++;
                        item3.SetValue(Grid.RowProperty, i + 1);
                    }
                }
            }
            item.Data = DataContext;
            item.IndexData = IndexData;
            item.IsMouseOverLineChanged += this.item_IsMouseOverLineChanged;
            item.ContextMenu = this.Parent.ContextmenuRow;
            item.BorderBrush = this.Parent.HorizontalBorderColor;
            item.SetValue(Grid.ColumnProperty, 0);
            if (ListCol.Count() > 0)
            {
                item.SetValue(Grid.ColumnSpanProperty, ListCol.Count());
            }
            item.SetValue(Grid.RowProperty, Index);
            this.SetupEvents(item);
            if (IndexData % 2 == 0)
            {
                item.SetValue(JFCGridItem.IsAlternateProperty, true);
            }
            grid.Children.Add(item);
            int num2 = 0;
            foreach (JFCGridColumn item4 in ListCol)
            {
                this.SetupCellData(grid, linkedList, item4, item, num2, Index, DataContext);
                num2++;
            }
            JFCBorder jFCBorder = new JFCBorder();
            jFCBorder.BorderBrush = this.Parent.HorizontalBorderColor;
            jFCBorder.BorderThickness = new Thickness(0.0, 0.0, 0.0, 1.0);
            jFCBorder.Orientation = Orientation.Horizontal;
            jFCBorder.ContextMenu = item.ContextMenu;
            jFCBorder.SetValue(Grid.RowProperty, Index);
            if (ListCol.Count() > 0)
            {
                jFCBorder.SetValue(Grid.ColumnSpanProperty, ListCol.Count());
            }
            this.SetupEvents(jFCBorder);
            grid.Children.Add(jFCBorder);
            linkedList.AddLast(jFCBorder);
            TabLineGrid.Insert(Index, linkedList);
        }

        private void RowAddGridVirtual(Grid grid, JFCGridItem item, int Index, int IndexData, List<LinkedList<UIElement>> TabLineGrid, JFCExpendItem DataContext, IList<JFCGridColumn> ListCol)
        {
            RowDefinition rowDefinition = new RowDefinition();
            rowDefinition.Height = new GridLength((double)this.Parent.RowHeight);
            grid.RowDefinitions.Insert(Index, rowDefinition);
            int num = grid.RowDefinitions.Count();
            LinkedList<UIElement> linkedList = new LinkedList<UIElement>();
            linkedList.AddLast(item);
            foreach (UIElement item2 in grid.Children.Cast<UIElement>().Where(delegate (UIElement ctrl)
            {
                if (!(ctrl is JFCBorder))
                {
                    return ctrl is JFCGridItem;
                }
                return true;
            }))
            {
                if (item2 is JFCBorder)
                {
                    if ((item2 as JFCBorder).Orientation == Orientation.Vertical)
                    {
                        item2.SetValue(Grid.RowProperty, 0);
                        item2.SetValue(Grid.RowSpanProperty, num);
                    }
                }
                else if (item2 is JFCGridItem && (item2 as JFCGridItem).Orientation == Orientation.Vertical)
                {
                    item2.SetValue(Grid.RowProperty, 0);
                    item2.SetValue(Grid.RowSpanProperty, num);
                }
            }
            for (int i = Index; i < TabLineGrid.Count(); i++)
            {
                foreach (UIElement item3 in TabLineGrid[i])
                {
                    if (item3 is JFCBorder)
                    {
                        if ((item3 as JFCBorder).Orientation != Orientation.Vertical)
                        {
                            item3.SetValue(Grid.RowProperty, i + 1);
                        }
                    }
                    else if (item3 is JFCGridItem)
                    {
                        if ((item3 as JFCGridItem).Orientation != Orientation.Vertical)
                        {
                            item3.SetValue(Grid.RowProperty, i + 1);
                        }
                    }
                    else
                    {
                        i++;
                        item3.SetValue(Grid.RowProperty, i + 1);
                    }
                }
            }
            item.Data = DataContext;
            item.IndexData = IndexData;
            item.IsMouseOverLineChanged += this.item_IsMouseOverLineChanged;
            item.ContextMenu = this.Parent.ContextmenuRow;
            item.BorderBrush = this.Parent.HorizontalBorderColor;
            item.SetValue(Grid.ColumnProperty, 0);
            if (grid.ColumnDefinitions.Count() > 0)
            {
                item.SetValue(Grid.ColumnSpanProperty, grid.ColumnDefinitions.Count());
            }
            item.SetValue(Grid.RowProperty, Index);
            this.SetupEvents(item);
            if (IndexData % 2 == 0)
            {
                item.SetValue(JFCGridItem.IsAlternateProperty, true);
            }
            grid.Children.Add(item);
            int num2 = 0;
            int num3 = ListCol.Count();
            int num4 = this.StartIndexColView;
            while (num4 <= this.StartIndexColView + grid.ColumnDefinitions.Count() - 1)
            {
                if (num4 < num3)
                {
                    JFCGridColumn col = ListCol[num4];
                    this.SetupCellData(grid, linkedList, col, item, num2, Index, DataContext);
                    num2++;
                    num4++;
                    continue;
                }
                throw new Exception("Nombre de colonnes du grid plus importante que les colonnes déclarées dans la JFCGrid");
            }
            JFCBorder jFCBorder = new JFCBorder();
            jFCBorder.BorderBrush = this.Parent.HorizontalBorderColor;
            jFCBorder.BorderThickness = new Thickness(0.0, 0.0, 0.0, 1.0);
            jFCBorder.Orientation = Orientation.Horizontal;
            jFCBorder.ContextMenu = item.ContextMenu;
            jFCBorder.SetValue(Grid.RowProperty, Index);
            if (grid.ColumnDefinitions.Count() > 0)
            {
                jFCBorder.SetValue(Grid.ColumnSpanProperty, grid.ColumnDefinitions.Count());
            }
            this.SetupEvents(jFCBorder);
            grid.Children.Add(jFCBorder);
            linkedList.AddLast(jFCBorder);
            TabLineGrid.Insert(Index, linkedList);
        }

        private void SetupCellData(Grid grid, LinkedList<UIElement> lUIElement, JFCGridColumn Col, JFCGridItem Item, int IdxCol, int IdxRow, JFCExpendItem DataContext)
        {
            bool flag = false;
            if (Col.CellTemplate == null)
            {
                CellImg cellImg;
                if (this.LstRemoveColCellImg.Count() > 0 && this.LstRemoveColCellImg.Last().Parent == grid)
                {
                    cellImg = this.LstRemoveColCellImg.Last();
                    this.LstRemoveColCellImg.Remove(cellImg);
                    flag = true;
                }
                else if (this.LstRecycleCellImg.Count() > 0)
                {
                    cellImg = this.LstRecycleCellImg.Last();
                    this.LstRecycleCellImg.RemoveAt(this.LstRecycleCellImg.Count() - 1);
                }
                else
                {
                    cellImg = new CellImg();
                }
                cellImg.Tag = null;
                cellImg.HorizontalAlignment = HorizontalAlignment.Stretch;
                cellImg.VerticalAlignment = VerticalAlignment.Stretch;
                cellImg.Column = Col;
                cellImg.PropertyChanged += delegate (object sender, PropertyChangedEventArgs e)
                {
                    if (e.PropertyName == "Data")
                    {
                        CellImg c = sender as CellImg;
                        if (c.Data != null)
                        {
                            object obj = default(object);
                            if (((JFCGridColumnBase)c.Column).GetCellValue(c.Data, out obj))
                            {
                                c.Text = obj.ToString();
                                if (c.Data is INotifyPropertyChanged)
                                {
                                    (c.Data as INotifyPropertyChanged).PropertyChanged += delegate (object sender1, PropertyChangedEventArgs e1)
                                    {
                                        c.Dispatcher.BeginInvoke((Action)delegate
                                        {
                                            if (e1.PropertyName == c.Column.BindingPath)
                                            {
                                                object obj2 = default(object);
                                                ((JFCGridColumnBase)c.Column).GetCellValue(c.Data, out obj2);
                                                if (obj2 != null)
                                                {
                                                    c.Text = obj2.ToString();
                                                }
                                                else
                                                {
                                                    c.Text = "";
                                                }
                                            }
                                        });
                                    };
                                }
                            }
                            else if (c.Column.BindingPath != null)
                            {
                                PropertyInfo data = c.Data.GetType().GetProperty(c.Column.BindingPath);
                                if (data != (PropertyInfo)null)
                                {
                                    if (c.Data is INotifyPropertyChanged)
                                    {
                                        (c.Data as INotifyPropertyChanged).PropertyChanged += delegate (object sender1, PropertyChangedEventArgs e1)
                                        {
                                            if (e1.PropertyName == c.Column.BindingPath)
                                            {
                                                c.Dispatcher.BeginInvoke((Action)delegate
                                                {
                                                    object value2 = data.GetValue(c.Data, null);
                                                    if (value2 != null)
                                                    {
                                                        c.Text = value2.ToString();
                                                    }
                                                    else
                                                    {
                                                        c.Text = "";
                                                    }
                                                });
                                            }
                                        };
                                    }
                                    object value = data.GetValue(c.Data, null);
                                    if (value != null)
                                    {
                                        c.Text = value.ToString();
                                    }
                                    else
                                    {
                                        c.Text = "";
                                    }
                                }
                                else
                                {
                                    Binding binding = new Binding(c.Column.BindingPath);
                                    binding.Source = c.Data;
                                    c.SetBinding(CellImg.TextProperty, binding);
                                }
                            }
                            else
                            {
                                Binding binding2 = new Binding();
                                binding2.Source = c.Data;
                                c.SetBinding(CellImg.TextProperty, binding2);
                            }
                        }
                        else
                        {
                            c.Text = "";
                        }
                    }
                };
                cellImg.SetValue(Grid.ColumnProperty, IdxCol);
                cellImg.SetValue(Grid.RowProperty, IdxRow);
                this.ApplyStyleCell(cellImg, Col);
                this.SetupEvents(cellImg);
                cellImg.ContextMenu = Item.ContextMenu;
                if (Col.TypeColumn == JFCGridColumn.TypesColumn.Normal)
                {
                    if (!flag)
                    {
                        if (cellImg.Parent is Grid)
                        {
                            ((Grid)cellImg.Parent).Children.Remove(cellImg);
                        }
                        grid.Children.Add(cellImg);
                    }
                    lUIElement.AddLast(cellImg);
                }
                else if (Col.TypeColumn == JFCGridColumn.TypesColumn.Hierarchical)
                {
                    HierarchicalItem hierarchicalItem = new HierarchicalItem();
                    hierarchicalItem.Component = cellImg;
                    hierarchicalItem.DataContext = DataContext;
                    this.DeleteEvents(cellImg, true);
                    hierarchicalItem.SetValue(Grid.ColumnProperty, IdxCol);
                    hierarchicalItem.SetValue(Grid.RowProperty, IdxRow);
                    this.SetupEvents(hierarchicalItem);
                    if (flag)
                    {
                        grid.Children.Remove(cellImg);
                    }
                    grid.Children.Add(hierarchicalItem);
                    lUIElement.AddLast(hierarchicalItem);
                }
                else
                {
                    if (Col.TypeColumn != JFCGridColumn.TypesColumn.GroupingNoRow && Col.TypeColumn != JFCGridColumn.TypesColumn.GroupingWithRow && Col.TypeColumn != JFCGridColumn.TypesColumn.GroupingWithRowWhenNoChildren)
                    {
                        return;
                    }
                    int indexData = this.Parent.ScrollVerticalValue + IdxRow;
                    JFCExpendItem jFCExpendItem = DataContext;
                    if (Col.LevelGrouping >= 0)
                    {
                        while (jFCExpendItem != null && jFCExpendItem.Level != Col.LevelGrouping)
                        {
                            jFCExpendItem = jFCExpendItem.Parent;
                        }
                    }
                    else
                    {
                        jFCExpendItem = null;
                    }
                    JFCGridCellGrouping jFCGridCellGrouping = new JFCGridCellGrouping();
                    jFCGridCellGrouping.Component = cellImg;
                    jFCGridCellGrouping.DataContext = jFCExpendItem;
                    this.DeleteEvents(cellImg, true);
                    if (jFCExpendItem != null && this.Parent.SelectedExpendItems.Contains(jFCExpendItem))
                    {
                        jFCGridCellGrouping.IsSelected = true;
                    }
                    jFCGridCellGrouping.SetValue(Grid.ColumnProperty, IdxCol);
                    jFCGridCellGrouping.SetValue(Grid.RowProperty, IdxRow);
                    jFCGridCellGrouping.SetValue(Panel.ZIndexProperty, 1000);
                    this.UpdateCellGroupingSizeAndVisibility(jFCGridCellGrouping, indexData, Col, jFCExpendItem);
                    this.SetupEvents(jFCGridCellGrouping);
                    if (flag)
                    {
                        grid.Children.Remove(cellImg);
                    }
                    grid.Children.Add(jFCGridCellGrouping);
                    lUIElement.AddLast(jFCGridCellGrouping);
                }
            }
            else
            {
                JFCGridCell jFCGridCell;
                if (this.LstRemoveColJFCGridCell.Count() > 0 && this.LstRemoveColJFCGridCell.Last().Parent == grid)
                {
                    jFCGridCell = this.LstRemoveColJFCGridCell.Last();
                    this.LstRemoveColJFCGridCell.RemoveAt(this.LstRemoveColJFCGridCell.Count() - 1);
                    flag = true;
                }
                else if (this.LstRecycleJFCGridCell.Count() > 0)
                {
                    jFCGridCell = this.LstRecycleJFCGridCell.Last();
                    this.LstRecycleJFCGridCell.RemoveAt(this.LstRecycleJFCGridCell.Count() - 1);
                }
                else
                {
                    jFCGridCell = new JFCGridCell();
                }
                jFCGridCell.Tag = null;
                jFCGridCell.HorizontalAlignment = HorizontalAlignment.Stretch;
                jFCGridCell.VerticalAlignment = VerticalAlignment.Stretch;
                jFCGridCell.ItemRow = Item;
                jFCGridCell.ItemColumn = Col.Item;
                jFCGridCell.Template = Col.CellTemplate;
                jFCGridCell.SetValue(Grid.ColumnProperty, IdxCol);
                jFCGridCell.SetValue(Grid.RowProperty, IdxRow);
                this.SetupEvents(jFCGridCell);
                jFCGridCell.ContextMenu = Item.ContextMenu;
                if (Col.TypeColumn == JFCGridColumn.TypesColumn.Normal)
                {
                    if (!flag && grid != jFCGridCell.Parent)
                    {
                        grid.Children.Add(jFCGridCell);
                    }
                    lUIElement.AddLast(jFCGridCell);
                }
                else if (Col.TypeColumn == JFCGridColumn.TypesColumn.Hierarchical)
                {
                    HierarchicalItem hierarchicalItem2 = new HierarchicalItem();
                    hierarchicalItem2.Component = jFCGridCell;
                    hierarchicalItem2.DataContext = DataContext;
                    this.DeleteEvents(jFCGridCell, true);
                    hierarchicalItem2.SetValue(Grid.ColumnProperty, IdxCol);
                    hierarchicalItem2.SetValue(Grid.RowProperty, IdxRow);
                    this.SetupEvents(hierarchicalItem2);
                    if (flag)
                    {
                        grid.Children.Remove(jFCGridCell);
                    }
                    grid.Children.Add(hierarchicalItem2);
                    lUIElement.AddLast(hierarchicalItem2);
                }
                else
                {
                    if (Col.TypeColumn != JFCGridColumn.TypesColumn.GroupingNoRow && Col.TypeColumn != JFCGridColumn.TypesColumn.GroupingWithRow && Col.TypeColumn != JFCGridColumn.TypesColumn.GroupingWithRowWhenNoChildren)
                    {
                        return;
                    }
                    int indexData2 = this.Parent.ScrollVerticalValue + IdxRow;
                    JFCExpendItem jFCExpendItem2 = DataContext;
                    if (Col.LevelGrouping >= 0)
                    {
                        while (jFCExpendItem2 != null && jFCExpendItem2.Level != Col.LevelGrouping)
                        {
                            jFCExpendItem2 = jFCExpendItem2.Parent;
                        }
                    }
                    else
                    {
                        jFCExpendItem2 = null;
                    }
                    JFCGridCellGrouping jFCGridCellGrouping2 = new JFCGridCellGrouping();
                    jFCGridCellGrouping2.Component = jFCGridCell;
                    jFCGridCellGrouping2.DataContext = jFCExpendItem2;
                    this.DeleteEvents(jFCGridCell, true);
                    if (jFCExpendItem2 != null && this.Parent.SelectedExpendItems.Contains(jFCExpendItem2))
                    {
                        jFCGridCellGrouping2.IsSelected = true;
                    }
                    jFCGridCellGrouping2.SetValue(Grid.ColumnProperty, IdxCol);
                    jFCGridCellGrouping2.SetValue(Grid.RowProperty, IdxRow);
                    jFCGridCellGrouping2.SetValue(Panel.ZIndexProperty, 1000);
                    this.UpdateCellGroupingSizeAndVisibility(jFCGridCellGrouping2, indexData2, Col, jFCExpendItem2);
                    this.SetupEvents(jFCGridCellGrouping2);
                    if (flag)
                    {
                        grid.Children.Remove(jFCGridCell);
                    }
                    grid.Children.Add(jFCGridCellGrouping2);
                    lUIElement.AddLast(jFCGridCellGrouping2);
                }
            }
        }

        private void RowMoveTab(List<LinkedList<UIElement>> TabLine, int IndexBefore, int IndexAfter)
        {
            int num = 0;
            LinkedList<UIElement>.Enumerator enumerator;
            if (IndexBefore < IndexAfter)
            {
                for (int i = IndexBefore; i < IndexAfter; i++)
                {
                    enumerator = TabLine[i].GetEnumerator();
                    try
                    {
                        while (enumerator.MoveNext())
                        {
                            UIElement current = enumerator.Current;
                            if (current is JFCBorder)
                            {
                                if ((current as JFCBorder).Orientation == Orientation.Horizontal)
                                {
                                    num = (int)current.GetValue(Grid.RowProperty);
                                    num--;
                                    current.SetValue(Grid.RowProperty, num);
                                }
                            }
                            else if (current is JFCGridItem)
                            {
                                this.MyElement_MouseLeave(current, null);
                                num = (int)current.GetValue(Grid.RowProperty);
                                num--;
                                current.SetValue(Grid.RowProperty, num);
                            }
                            else if (current is JFCGridCellGrouping)
                            {
                                num = (int)current.GetValue(Grid.RowProperty);
                                num--;
                                current.SetValue(Grid.RowProperty, num);
                                int indexData = this.Parent.ScrollVerticalValue + num;
                                JFCGridCellGrouping jFCGridCellGrouping = current as JFCGridCellGrouping;
                                JFCExpendItem jFCExpendItem = jFCGridCellGrouping.DataContext as JFCExpendItem;
                                if (jFCExpendItem != null)
                                {
                                    this.UpdateCellGroupingSizeAndVisibility(jFCGridCellGrouping, indexData, null, jFCExpendItem);
                                }
                            }
                            else
                            {
                                num = (int)current.GetValue(Grid.RowProperty);
                                num--;
                                current.SetValue(Grid.RowProperty, num);
                            }
                        }
                    }
                    finally
                    {
                        ((IDisposable)enumerator).Dispose();
                    }
                }
            }
            else if (IndexBefore > IndexAfter)
            {
                for (int num2 = IndexBefore; num2 > IndexAfter; num2--)
                {
                    enumerator = TabLine[num2].GetEnumerator();
                    try
                    {
                        while (enumerator.MoveNext())
                        {
                            UIElement current2 = enumerator.Current;
                            if (current2 is JFCBorder)
                            {
                                if ((current2 as JFCBorder).Orientation == Orientation.Horizontal)
                                {
                                    num = (int)current2.GetValue(Grid.RowProperty);
                                    num++;
                                    current2.SetValue(Grid.RowProperty, num);
                                }
                            }
                            else if (current2 is JFCGridItem)
                            {
                                this.MyElement_MouseLeave(current2, null);
                                num = (int)current2.GetValue(Grid.RowProperty);
                                num++;
                                current2.SetValue(Grid.RowProperty, num);
                            }
                            else if (current2 is JFCGridCellGrouping)
                            {
                                num = (int)current2.GetValue(Grid.RowProperty);
                                num++;
                                current2.SetValue(Grid.RowProperty, num);
                                int indexData2 = this.Parent.ScrollVerticalValue + num;
                                JFCGridCellGrouping jFCGridCellGrouping2 = current2 as JFCGridCellGrouping;
                                JFCExpendItem jFCExpendItem2 = jFCGridCellGrouping2.DataContext as JFCExpendItem;
                                if (jFCExpendItem2 != null)
                                {
                                    this.UpdateCellGroupingSizeAndVisibility(jFCGridCellGrouping2, indexData2, null, jFCExpendItem2);
                                }
                            }
                            else
                            {
                                num = (int)current2.GetValue(Grid.RowProperty);
                                num++;
                                current2.SetValue(Grid.RowProperty, num);
                            }
                        }
                    }
                    finally
                    {
                        ((IDisposable)enumerator).Dispose();
                    }
                }
            }
            enumerator = TabLine[IndexAfter].GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    UIElement current3 = enumerator.Current;
                    if (current3 is JFCBorder)
                    {
                        if ((current3 as JFCBorder).Orientation == Orientation.Horizontal)
                        {
                            current3.SetValue(Grid.RowProperty, IndexAfter);
                        }
                    }
                    else if (current3 is JFCGridItem)
                    {
                        this.MyElement_MouseLeave(current3, null);
                        current3.SetValue(Grid.RowProperty, IndexAfter);
                    }
                    else if (current3 is JFCGridCellGrouping)
                    {
                        current3.SetValue(Grid.RowProperty, IndexAfter);
                    }
                    else
                    {
                        current3.SetValue(Grid.RowProperty, IndexAfter);
                    }
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
        }

        public void RowMove(int IndexBefore, int IndexAfter)
        {
            if ((IndexBefore >= IndexAfter || (this.TabLineH.Count() > IndexAfter && this.TabLineB.Count() > IndexAfter && this.TabLineF.Count() > IndexAfter)) && (IndexBefore <= IndexAfter || (this.TabLineH.Count() > IndexBefore && this.TabLineB.Count() > IndexBefore && this.TabLineF.Count() > IndexBefore)))
            {
                LinkedList<UIElement> item = this.TabLineH[IndexBefore];
                this.TabLineH.RemoveAt(IndexBefore);
                this.TabLineH.Insert(IndexAfter, item);
                LinkedList<UIElement> item2 = this.TabLineB[IndexBefore];
                this.TabLineB.RemoveAt(IndexBefore);
                this.TabLineB.Insert(IndexAfter, item2);
                LinkedList<UIElement> item3 = this.TabLineF[IndexBefore];
                this.TabLineF.RemoveAt(IndexBefore);
                this.TabLineF.Insert(IndexAfter, item3);
                this.RowMoveTab(this.TabLineH, IndexBefore, IndexAfter);
                this.RowMoveTab(this.TabLineB, IndexBefore, IndexAfter);
                this.RowMoveTab(this.TabLineF, IndexBefore, IndexAfter);
            }
        }

        public void RowMoveBegin(int Index)
        {
            this.RowMove(Index, 0);
        }

        public void RowMoveEnd(int Index)
        {
            int indexAfter = this.Grid11.RowDefinitions.Count() - 1;
            this.RowMove(Index, indexAfter);
        }

        public void RowMoveBeginToEnd()
        {
            int num = this.Grid11.RowDefinitions.Count() - 1;
            this.RowRefreshData(num, -1);
            this.RowMove(0, num);
            this.RowRefreshData(num);
        }

        public void RowMoveBeginToEnd(int NbRow)
        {
            int num = this.Grid11.RowDefinitions.Count() - 1;
            for (int num2 = NbRow - 1; num2 >= 0; num2--)
            {
                this.RowMove(0, num);
                this.RowRefreshData(num, this.Parent.ScrollVerticalValue + num - num2);
            }
        }

        public void RowMoveEndToBegin()
        {
            int num = this.Grid11.RowDefinitions.Count() - 1;
            this.RowRefreshData(num, -1);
            this.RowMove(num, 0);
            this.RowRefreshData(0);
        }

        public void RowMoveEndToBegin(int NbRow)
        {
            int num = this.Grid11.RowDefinitions.Count() - 1;
            for (int num2 = NbRow - 1; num2 >= 0; num2--)
            {
                this.RowRefreshData(num, -1);
                this.RowMove(num, 0);
                this.RowRefreshData(0, this.Parent.ScrollVerticalValue + num2);
            }
        }

        private void RowRemoveTab(List<LinkedList<UIElement>> TabLine, int Index, Grid gridtmp, int NbRow)
        {
            if (TabLine.Count() > Index)
            {
                LinkedList<UIElement>.Enumerator enumerator = TabLine[Index].GetEnumerator();
                try
                {
                    while (enumerator.MoveNext())
                    {
                        UIElement current = enumerator.Current;
                        if (current is JFCBorder)
                        {
                            if ((current as JFCBorder).Orientation == Orientation.Horizontal)
                            {
                                gridtmp.Children.Remove(current);
                                this.DeleteEvents(current, false);
                            }
                        }
                        else
                        {
                            gridtmp.Children.Remove(current);
                            this.DeleteEvents(current, false);
                            if (current is CellImg)
                            {
                                this.LstRecycleCellImg.Add((CellImg)current);
                            }
                            else if (current is JFCGridCell)
                            {
                                this.LstRecycleJFCGridCell.Add((JFCGridCell)current);
                            }
                        }
                    }
                }
                finally
                {
                    ((IDisposable)enumerator).Dispose();
                }
                gridtmp.RowDefinitions.RemoveAt(Index);
                for (int i = Index + 1; i < TabLine.Count(); i++)
                {
                    enumerator = TabLine[i].GetEnumerator();
                    try
                    {
                        while (enumerator.MoveNext())
                        {
                            UIElement current2 = enumerator.Current;
                            int num = i - 1;
                            current2.SetValue(Grid.RowProperty, num);
                        }
                    }
                    finally
                    {
                        ((IDisposable)enumerator).Dispose();
                    }
                }
                foreach (UIElement item in from UIElement ctrl in gridtmp.Children
                                           where ctrl is JFCBorder
                                           select ctrl)
                {
                    if ((item as JFCBorder).Orientation == Orientation.Vertical)
                    {
                        item.SetValue(Grid.RowSpanProperty, NbRow);
                    }
                }
            }
        }

        public void RowRemove(int Index)
        {
            int nbRow = this.Grid11.RowDefinitions.Count();
            this.RowRemoveTab(this.TabLineH, Index, this.Grid10, nbRow);
            this.RowRemoveTab(this.TabLineB, Index, this.Grid11, nbRow);
            this.RowRemoveTab(this.TabLineF, Index, this.Grid12, nbRow);
            this.TabLineH.RemoveAt(Index);
            this.TabLineB.RemoveAt(Index);
            this.TabLineF.RemoveAt(Index);
        }

        public void RowFooterRemove(int Index)
        {
            int num = this.Grid11.RowDefinitions.Count();
            LinkedList<UIElement>.Enumerator enumerator = this.TabLineBF[Index].GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    UIElement current = enumerator.Current;
                    if (current is JFCBorder)
                    {
                        if ((current as JFCBorder).Orientation == Orientation.Horizontal)
                        {
                            this.Grid21.Children.Remove(current);
                            this.DeleteEvents(current, false);
                        }
                    }
                    else
                    {
                        this.Grid21.Children.Remove(current);
                        this.DeleteEvents(current, false);
                        if (current is CellImg)
                        {
                            this.LstRecycleCellImg.Add((CellImg)current);
                        }
                        else if (current is JFCGridCell)
                        {
                            this.LstRecycleJFCGridCell.Add((JFCGridCell)current);
                        }
                    }
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            this.Grid21.RowDefinitions.RemoveAt(Index);
            for (int i = Index + 1; i < this.TabLineBF.Count(); i++)
            {
                enumerator = this.TabLineBF[i].GetEnumerator();
                try
                {
                    while (enumerator.MoveNext())
                    {
                        UIElement current2 = enumerator.Current;
                        int num2 = i - 1;
                        current2.SetValue(Grid.RowProperty, num2);
                    }
                }
                finally
                {
                    ((IDisposable)enumerator).Dispose();
                }
            }
            foreach (UIElement item in from UIElement ctrl in this.Grid21.Children
                                       where ctrl is JFCBorder
                                       select ctrl)
            {
                if ((item as JFCBorder).Orientation == Orientation.Vertical)
                {
                    item.SetValue(Grid.RowSpanProperty, num);
                }
            }
            this.TabLineBF.RemoveAt(Index);
        }

        public void RowRemoveAll()
        {
            this.TabLineH.Clear();
            this.TabLineB.Clear();
            this.TabLineF.Clear();
            for (int i = 0; i < this.Grid10.Children.Count; i++)
            {
                UIElement uIElement = this.Grid10.Children[i];
                if (uIElement is JFCBorder)
                {
                    if (((JFCBorder)uIElement).Orientation == Orientation.Horizontal)
                    {
                        this.Grid10.Children.Remove(uIElement);
                        i--;
                        this.DeleteEvents(uIElement, false);
                    }
                }
                else
                {
                    this.Grid10.Children.Remove(uIElement);
                    i--;
                    this.DeleteEvents(uIElement, false);
                    if (uIElement is CellImg)
                    {
                        this.LstRecycleCellImg.Add((CellImg)uIElement);
                    }
                    else if (uIElement is JFCGridCell)
                    {
                        this.LstRecycleJFCGridCell.Add((JFCGridCell)uIElement);
                    }
                }
            }
            for (int j = 0; j < this.Grid11.Children.Count; j++)
            {
                UIElement uIElement2 = this.Grid11.Children[j];
                if (uIElement2 is JFCBorder)
                {
                    if (((JFCBorder)uIElement2).Orientation == Orientation.Horizontal)
                    {
                        this.Grid11.Children.Remove(uIElement2);
                        j--;
                        this.DeleteEvents(uIElement2, false);
                    }
                }
                else
                {
                    this.Grid11.Children.Remove(uIElement2);
                    j--;
                    this.DeleteEvents(uIElement2, false);
                    if (uIElement2 is CellImg)
                    {
                        this.LstRecycleCellImg.Add((CellImg)uIElement2);
                    }
                    else if (uIElement2 is JFCGridCell)
                    {
                        this.LstRecycleJFCGridCell.Add((JFCGridCell)uIElement2);
                    }
                }
            }
            for (int k = 0; k < this.Grid12.Children.Count; k++)
            {
                UIElement uIElement3 = this.Grid12.Children[k];
                if (uIElement3 is JFCBorder)
                {
                    if (((JFCBorder)uIElement3).Orientation == Orientation.Horizontal)
                    {
                        this.Grid12.Children.Remove(uIElement3);
                        k--;
                        this.DeleteEvents(uIElement3, false);
                    }
                }
                else
                {
                    this.Grid12.Children.Remove(uIElement3);
                    k--;
                    this.DeleteEvents(uIElement3, false);
                    if (uIElement3 is CellImg)
                    {
                        this.LstRecycleCellImg.Add((CellImg)uIElement3);
                    }
                    else if (uIElement3 is JFCGridCell)
                    {
                        this.LstRecycleJFCGridCell.Add((JFCGridCell)uIElement3);
                    }
                }
            }
            this.Grid10.RowDefinitions.Clear();
            this.Grid11.RowDefinitions.Clear();
            this.Grid12.RowDefinitions.Clear();
        }

        public void RowFooterRemoveAll()
        {
            this.TabLineHF.Clear();
            this.TabLineBF.Clear();
            for (int i = 0; i < this.Grid20.Children.Count; i++)
            {
                UIElement uIElement = this.Grid20.Children[i];
                if (uIElement is JFCBorder)
                {
                    if (((JFCBorder)uIElement).Orientation == Orientation.Horizontal)
                    {
                        this.Grid20.Children.Remove(uIElement);
                        i--;
                        this.DeleteEvents(uIElement, false);
                    }
                }
                else
                {
                    this.Grid20.Children.Remove(uIElement);
                    i--;
                    this.DeleteEvents(uIElement, false);
                    if (uIElement is CellImg)
                    {
                        this.LstRecycleCellImg.Add((CellImg)uIElement);
                    }
                    else if (uIElement is JFCGridCell)
                    {
                        this.LstRecycleJFCGridCell.Add((JFCGridCell)uIElement);
                    }
                }
            }
            for (int j = 0; j < this.Grid21.Children.Count; j++)
            {
                UIElement uIElement2 = this.Grid21.Children[j];
                if (uIElement2 is JFCBorder)
                {
                    if (((JFCBorder)uIElement2).Orientation == Orientation.Horizontal)
                    {
                        this.Grid21.Children.Remove(uIElement2);
                        j--;
                        this.DeleteEvents(uIElement2, false);
                    }
                }
                else
                {
                    this.Grid21.Children.Remove(uIElement2);
                    j--;
                    this.DeleteEvents(uIElement2, false);
                    if (uIElement2 is CellImg)
                    {
                        this.LstRecycleCellImg.Add((CellImg)uIElement2);
                    }
                    else if (uIElement2 is JFCGridCell)
                    {
                        this.LstRecycleJFCGridCell.Add((JFCGridCell)uIElement2);
                    }
                }
            }
            this.Grid20.RowDefinitions.Clear();
            this.Grid21.RowDefinitions.Clear();
        }

        public void RowRefreshDataAll()
        {
            JFCGrid parent = this.Parent;
            if (((parent != null) ? parent.dataSource : null) != null)
            {
                int num = this.Parent.dataSource.Count();
                for (int i = 0; i < this.Grid11.RowDefinitions.Count(); i++)
                {
                    if (this.Parent.ScrollVerticalValue + i >= num)
                    {
                        this.RowRemove(i);
                        i--;
                    }
                    else
                    {
                        this.RowRefreshData(i);
                    }
                }
            }
            else
            {
                this.RowRemoveAll();
            }
        }

        public void RowFooterRefreshDataAll()
        {
            JFCGrid parent = this.Parent;
            if (((parent != null) ? parent.dataSourceFooter : null) != null)
            {
                int num = this.Parent.dataSourceFooter.Count();
                for (int i = 0; i < this.Grid21.RowDefinitions.Count(); i++)
                {
                    if (this.Grid21.RowDefinitions.Count > num)
                    {
                        this.RowFooterRemove(i);
                        i--;
                    }
                    else
                    {
                        this.RowFooterRefreshData(i);
                    }
                }
            }
            else
            {
                this.RowFooterRemoveAll();
            }
        }

        public void RowRefreshDataAllAsync()
        {
            int num = this.Parent.dataSource.Count();
            int i;
            for (i = 0; i < 10; i++)
            {
                if (this.Parent.ScrollVerticalValue + i >= num)
                {
                    this.RowRemove(i);
                    i--;
                }
                else
                {
                    this.RowRefreshData(i);
                }
            }
            this.timer.Interval = new TimeSpan(0, 0, 0, 0, 100);
            this.timer.Tick -= this.timer_Tick;
            this.timer.Tag = i;
            this.timer.Tick += this.timer_Tick;
            this.timer.IsEnabled = true;
        }

        private void timer_Tick(object sender, EventArgs e)
        {
            DispatcherTimer dispatcherTimer = sender as DispatcherTimer;
            int num = (int)dispatcherTimer.Tag;
            int num2 = this.Parent.dataSource.Count();
            for (int i = num; i < this.Grid11.RowDefinitions.Count(); i++)
            {
                if (this.Parent.ScrollVerticalValue + i >= num2)
                {
                    this.RowRemove(i);
                    i--;
                }
                else
                {
                    this.RowRefreshData(i);
                }
            }
            dispatcherTimer.IsEnabled = false;
        }

        public void RowRefreshData(int Index)
        {
            if (this.Parent.ScrollVerticalValue + Index >= this.Parent.dataSource.Count())
            {
                this.RowRemove(Index);
            }
            else
            {
                this.RowRefreshData(Index, this.Parent.ScrollVerticalValue + Index);
            }
        }

        public void RowRefreshData(int Index, int IndexData)
        {
            this.RowRefreshData(Index, IndexData, null);
        }

        public void RefreshData(int Index, int IndexData, JFCGridColumn Column, List<LinkedList<UIElement>> TabLine, JFCExpendItem DataContext, IList<JFCGridColumn> ListCol)
        {
            if (TabLine.Count > Index && Index >= 0)
            {
                int num = ListCol.Count();
                foreach (UIElement item in TabLine[Index])
                {
                    JFCGridColumn Col;
                    if (num > 0)
                    {
                        Col = null;
                        if (Column != null)
                        {
                            int num2 = (int)item.GetValue(Grid.ColumnProperty);
                            if (Column.Frozen == JFCGridColumn.FrozenType.Start)
                            {
                                Col = ListCol[num2];
                            }
                            else if (Column.Frozen == JFCGridColumn.FrozenType.End)
                            {
                                Col = ListCol[num2];
                            }
                            else
                            {
                                if (this.StartIndexColView + num2 >= ListCol.Count())
                                {
                                    break;
                                }
                                Col = ListCol[this.StartIndexColView + num2];
                            }
                            if (Column == Col)
                            {
                                goto IL_00f3;
                            }
                            continue;
                        }
                        goto IL_00f3;
                    }
                    goto IL_0ced;
                IL_00f3:
                    if (item is CellImg)
                    {
                        CellImg cellImg = item as CellImg;
                        if (DataContext == null)
                        {
                            cellImg.Data = null;
                        }
                        else
                        {
                            cellImg.Data = DataContext.Obj;
                            if (this.Parent.IsSearchVisible)
                            {
                                if (this.Parent.SearchResult.Count() > 0)
                                {
                                    if (Col == null)
                                    {
                                        int num3 = (int)item.GetValue(Grid.ColumnProperty);
                                        if (ListCol == this.ListColBody)
                                        {
                                            Col = ListCol[this.StartIndexColView + num3];
                                        }
                                        else
                                        {
                                            Col = ListCol[num3];
                                        }
                                    }
                                    if (this.Parent.SearchResult.Where(delegate (JFCGrid.GridPosition s)
                                    {
                                        if (s.Column == Col)
                                        {
                                            return s.Item == DataContext;
                                        }
                                        return false;
                                    }).Count() >= 1)
                                    {
                                        cellImg.HiliteText = this.Parent.TextBoxSearch.Text;
                                    }
                                    else
                                    {
                                        cellImg.HiliteText = "";
                                    }
                                }
                                else
                                {
                                    cellImg.HiliteText = "";
                                }
                            }
                            else
                            {
                                cellImg.HiliteText = "";
                            }
                        }
                    }
                    else if (item is JFCGridCell)
                    {
                        JFCGridCell jFCGridCell = item as JFCGridCell;
                        if (Col == null)
                        {
                            int num4 = (int)item.GetValue(Grid.ColumnProperty);
                            if (ListCol == this.ListColBody)
                            {
                                Col = ListCol[this.StartIndexColView + num4];
                            }
                            else
                            {
                                Col = ListCol[num4];
                            }
                        }
                        object dataContext = default(object);
                        if (DataContext != null && ((JFCGridColumnBase)Col).GetCellValue(DataContext.Obj, out dataContext))
                        {
                            jFCGridCell.DataContext = dataContext;
                        }
                        else if (Col.BindingPath != "" && Col.BindingPath != null && DataContext != null)
                        {
                            PropertyInfo property = DataContext.Obj.GetType().GetProperty(Col.BindingPath);
                            if (property != (PropertyInfo)null)
                            {
                                jFCGridCell.DataContext = property.GetValue(DataContext.Obj, null);
                            }
                            else
                            {
                                Binding binding = new Binding(Col.BindingPath);
                                binding.Source = DataContext.Obj;
                                jFCGridCell.SetBinding(FrameworkElement.DataContextProperty, binding);
                            }
                        }
                        else if (DataContext == null)
                        {
                            jFCGridCell.DataContext = null;
                        }
                        else
                        {
                            jFCGridCell.DataContext = DataContext.Obj;
                        }
                        if (this.Parent.IsSearchVisible)
                        {
                            if (this.Parent.SearchResult.Count() > 0)
                            {
                                if (this.Parent.SearchResult.Where(delegate (JFCGrid.GridPosition s)
                                {
                                    if (s.Column == Col)
                                    {
                                        return s.Item == DataContext;
                                    }
                                    return false;
                                }).Count() >= 1)
                                {
                                    jFCGridCell.HiliteText = this.Parent.TextBoxSearch.Text;
                                }
                                else
                                {
                                    jFCGridCell.HiliteText = "";
                                }
                            }
                            else
                            {
                                jFCGridCell.HiliteText = "";
                            }
                        }
                        else
                        {
                            jFCGridCell.HiliteText = "";
                        }
                        foreach (UIElement item2 in TabLine[Index])
                        {
                            if (item2 is JFCGridItem && ((JFCGridItem)item2).Orientation == Orientation.Horizontal)
                            {
                                jFCGridCell.ItemRow = (JFCGridItem)item2;
                                break;
                            }
                        }
                        jFCGridCell.ItemColumn = Col.Item;
                    }
                    else if (item is HierarchicalItem)
                    {
                        HierarchicalItem hierarchicalItem = item as HierarchicalItem;
                        hierarchicalItem.DataContext = DataContext;
                        if (hierarchicalItem.Component is CellImg)
                        {
                            CellImg cellImg = hierarchicalItem.Component as CellImg;
                            if (DataContext == null)
                            {
                                cellImg.Data = null;
                            }
                            else
                            {
                                cellImg.Data = DataContext.Obj;
                                if (this.Parent.IsSearchVisible)
                                {
                                    if (this.Parent.SearchResult.Count() > 0)
                                    {
                                        if (Col == null)
                                        {
                                            int num5 = (int)item.GetValue(Grid.ColumnProperty);
                                            if (ListCol == this.ListColBody)
                                            {
                                                Col = ListCol[this.StartIndexColView + num5];
                                            }
                                            else
                                            {
                                                Col = ListCol[num5];
                                            }
                                        }
                                        if (this.Parent.SearchResult.Where(delegate (JFCGrid.GridPosition s)
                                        {
                                            if (s.Column == Col)
                                            {
                                                return s.Item == DataContext;
                                            }
                                            return false;
                                        }).Count() >= 1)
                                        {
                                            cellImg.HiliteText = this.Parent.TextBoxSearch.Text;
                                        }
                                        else
                                        {
                                            cellImg.HiliteText = "";
                                        }
                                    }
                                    else
                                    {
                                        cellImg.HiliteText = "";
                                    }
                                }
                                else
                                {
                                    cellImg.HiliteText = "";
                                }
                            }
                        }
                        else
                        {
                            if (Col == null)
                            {
                                int num6 = (int)item.GetValue(Grid.ColumnProperty);
                                if (ListCol == this.ListColBody)
                                {
                                    Col = ListCol[this.StartIndexColView + num6];
                                }
                                else
                                {
                                    Col = ListCol[num6];
                                }
                            }
                            if (hierarchicalItem.Component is JFCGridCell)
                            {
                                JFCGridCell jFCGridCell2 = hierarchicalItem.Component as JFCGridCell;
                                if (this.Parent.IsSearchVisible)
                                {
                                    if (this.Parent.SearchResult.Count() > 0)
                                    {
                                        if (this.Parent.SearchResult.Where(delegate (JFCGrid.GridPosition s)
                                        {
                                            if (s.Column == Col)
                                            {
                                                return s.Item == DataContext;
                                            }
                                            return false;
                                        }).Count() >= 1)
                                        {
                                            jFCGridCell2.HiliteText = this.Parent.TextBoxSearch.Text;
                                        }
                                        else
                                        {
                                            jFCGridCell2.HiliteText = "";
                                        }
                                    }
                                    else
                                    {
                                        jFCGridCell2.HiliteText = "";
                                    }
                                }
                                else
                                {
                                    jFCGridCell2.HiliteText = "";
                                }
                            }
                            object dataContext2 = default(object);
                            if (DataContext != null && ((JFCGridColumnBase)Col).GetCellValue(DataContext.Obj, out dataContext2))
                            {
                                hierarchicalItem.Component.DataContext = dataContext2;
                            }
                            else if (Col.BindingPath != "" && Col.BindingPath != null && DataContext != null)
                            {
                                PropertyInfo property2 = DataContext.Obj.GetType().GetProperty(Col.BindingPath);
                                if (property2 != (PropertyInfo)null)
                                {
                                    hierarchicalItem.Component.DataContext = property2.GetValue(DataContext.Obj, null);
                                }
                                else
                                {
                                    Binding binding2 = new Binding(Col.BindingPath);
                                    binding2.Source = DataContext.Obj;
                                    hierarchicalItem.Component.SetBinding(FrameworkElement.DataContextProperty, binding2);
                                }
                            }
                            else if (DataContext == null)
                            {
                                hierarchicalItem.Component.DataContext = null;
                            }
                            else
                            {
                                hierarchicalItem.Component.DataContext = DataContext.Obj;
                            }
                        }
                    }
                    else if (item is JFCGridCellGrouping)
                    {
                        JFCGridCellGrouping jFCGridCellGrouping = item as JFCGridCellGrouping;
                        int num7 = (int)item.GetValue(Grid.ColumnProperty);
                        if (Col == null)
                        {
                            if (ListCol == this.ListColBody)
                            {
                                Col = ListCol[this.StartIndexColView + num7];
                            }
                            else
                            {
                                Col = ListCol[num7];
                            }
                        }
                        JFCExpendItem dataContextTmp = DataContext;
                        if (Col.LevelGrouping >= 0)
                        {
                            while (dataContextTmp != null && dataContextTmp.Level != Col.LevelGrouping)
                            {
                                dataContextTmp = dataContextTmp.Parent;
                            }
                        }
                        else
                        {
                            dataContextTmp = null;
                        }
                        jFCGridCellGrouping.DataContext = dataContextTmp;
                        if (dataContextTmp != null && this.Parent.SelectedExpendItems.Contains(dataContextTmp))
                        {
                            jFCGridCellGrouping.IsSelected = true;
                        }
                        jFCGridCellGrouping.SetValue(Grid.ColumnProperty, num7);
                        jFCGridCellGrouping.SetValue(Grid.RowProperty, Index);
                        jFCGridCellGrouping.SetValue(Panel.ZIndexProperty, 1000);
                        this.UpdateCellGroupingSizeAndVisibility(jFCGridCellGrouping, IndexData, Column, dataContextTmp);
                        if (this.Parent.SelectedExpendItems.Contains(dataContextTmp) || this.Parent.SelectedExpendItemsChildren.Contains(dataContextTmp))
                        {
                            if (!jFCGridCellGrouping.IsSelected)
                            {
                                jFCGridCellGrouping.IsSelected = true;
                            }
                        }
                        else if (jFCGridCellGrouping.IsSelected)
                        {
                            jFCGridCellGrouping.IsSelected = false;
                        }
                        if (jFCGridCellGrouping.Component is CellImg)
                        {
                            CellImg cellImg = jFCGridCellGrouping.Component as CellImg;
                            if (dataContextTmp == null)
                            {
                                cellImg.Data = null;
                            }
                            else
                            {
                                cellImg.Data = dataContextTmp.Obj;
                                if (this.Parent.IsSearchVisible)
                                {
                                    if (this.Parent.SearchResult.Count() > 0)
                                    {
                                        if (this.Parent.SearchResult.Where(delegate (JFCGrid.GridPosition s)
                                        {
                                            if (s.Column == Col)
                                            {
                                                return s.Item == dataContextTmp;
                                            }
                                            return false;
                                        }).Count() >= 1)
                                        {
                                            cellImg.HiliteText = this.Parent.TextBoxSearch.Text;
                                        }
                                        else
                                        {
                                            cellImg.HiliteText = "";
                                        }
                                    }
                                    else
                                    {
                                        cellImg.HiliteText = "";
                                    }
                                }
                                else
                                {
                                    cellImg.HiliteText = "";
                                }
                            }
                        }
                        else
                        {
                            if (jFCGridCellGrouping.Component is JFCGridCell)
                            {
                                JFCGridCell jFCGridCell3 = jFCGridCellGrouping.Component as JFCGridCell;
                                if (this.Parent.IsSearchVisible)
                                {
                                    if (this.Parent.SearchResult.Count() > 0)
                                    {
                                        if (this.Parent.SearchResult.Where(delegate (JFCGrid.GridPosition s)
                                        {
                                            if (s.Column == Col)
                                            {
                                                return s.Item == dataContextTmp;
                                            }
                                            return false;
                                        }).Count() >= 1)
                                        {
                                            jFCGridCell3.HiliteText = this.Parent.TextBoxSearch.Text;
                                        }
                                        else
                                        {
                                            jFCGridCell3.HiliteText = "";
                                        }
                                    }
                                    else
                                    {
                                        jFCGridCell3.HiliteText = "";
                                    }
                                }
                                else
                                {
                                    jFCGridCell3.HiliteText = "";
                                }
                            }
                            object dataContext3 = default(object);
                            if (dataContextTmp != null && ((JFCGridColumnBase)Col).GetCellValue(dataContextTmp.Obj, out dataContext3))
                            {
                                jFCGridCellGrouping.Component.DataContext = dataContext3;
                            }
                            else if (Col.BindingPath != "" && Col.BindingPath != null && dataContextTmp != null)
                            {
                                PropertyInfo property3 = dataContextTmp.Obj.GetType().GetProperty(Col.BindingPath);
                                if (property3 != (PropertyInfo)null)
                                {
                                    jFCGridCellGrouping.Component.DataContext = property3.GetValue(dataContextTmp.Obj, null);
                                }
                                else
                                {
                                    Binding binding3 = new Binding(Col.BindingPath);
                                    binding3.Source = dataContextTmp.Obj;
                                    jFCGridCellGrouping.Component.SetBinding(FrameworkElement.DataContextProperty, binding3);
                                }
                            }
                            else if (dataContextTmp == null)
                            {
                                jFCGridCellGrouping.Component.DataContext = null;
                            }
                            else
                            {
                                jFCGridCellGrouping.Component.DataContext = dataContextTmp.Obj;
                            }
                        }
                    }
                    goto IL_0ced;
                IL_0ced:
                    if (item is JFCGridItem)
                    {
                        JFCGridItem jFCGridItem = item as JFCGridItem;
                        jFCGridItem.Data = DataContext;
                        jFCGridItem.IndexData = IndexData;
                        if (IndexData % 2 == 0)
                        {
                            jFCGridItem.SetValue(JFCGridItem.IsAlternateProperty, true);
                        }
                        else
                        {
                            jFCGridItem.SetValue(JFCGridItem.IsAlternateProperty, false);
                        }
                        if (this.Parent.SelectedExpendItems.Contains(DataContext) || this.Parent.SelectedExpendItemsChildren.Contains(jFCGridItem.Data))
                        {
                            if (!jFCGridItem.IsSelected)
                            {
                                jFCGridItem.IsSelected = true;
                                jFCGridItem.SelectedPeer(true);
                            }
                        }
                        else if (jFCGridItem.IsSelected)
                        {
                            jFCGridItem.IsSelected = false;
                            jFCGridItem.SelectedPeer(false);
                        }
                    }
                    else if (item is JFCBorder)
                    {
                        JFCBorder jFCBorder = item as JFCBorder;
                        if (DataContext != null)
                        {
                            jFCBorder.BorderBrush = this.Parent.HorizontalBorderColor;
                        }
                    }
                }
            }
        }

        public void RowRefreshData(int Index, int IndexData, JFCGridColumn Column)
        {
            if (IndexData >= this.Parent.dataSource.Count())
            {
                this.RowRemove(Index);
            }
            else
            {
                JFCExpendItem dataContext = (IndexData >= 0) ? this.Parent.dataSource[IndexData] : null;
                this.RefreshData(Index, IndexData, Column, this.TabLineH, dataContext, this.ListColHeaderRow);
                this.RefreshData(Index, IndexData, Column, this.TabLineB, dataContext, this.ListColBody);
                this.RefreshData(Index, IndexData, Column, this.TabLineF, dataContext, this.ListColHeaderRowEnd);
            }
        }

        public void RowFooterRefreshData(int Index)
        {
            this.RowFooterRefreshData(Index, Index);
        }

        public void RowFooterRefreshData(int Index, int IndexData)
        {
            this.RowFooterRefreshData(Index, IndexData, null);
        }

        public void RowFooterRefreshData(int Index, int IndexData, JFCGridColumn Column)
        {
            if (IndexData >= this.Parent.dataSourceFooter.Count())
            {
                this.RowFooterRemove(Index);
            }
            else
            {
                JFCExpendItem dataContext = (IndexData >= 0) ? this.Parent.dataSourceFooter[IndexData] : null;
                this.RefreshData(Index, IndexData, Column, this.TabLineHF, dataContext, this.ListColHeaderFooter);
                this.RefreshData(Index, IndexData, Column, this.TabLineBF, dataContext, this.ListColBody);
            }
        }

        public void RefreshContextMenuRow()
        {
            this.RefreshContextMenuRow(this.TabLineH);
            this.RefreshContextMenuRow(this.TabLineHF);
            this.RefreshContextMenuRow(this.TabLineB);
            this.RefreshContextMenuRow(this.TabLineBF);
        }

        public void RefreshContextMenuRow(List<LinkedList<UIElement>> Tab)
        {
            for (int i = 0; i < Tab.Count(); i++)
            {
                foreach (UIElement item in Tab[i])
                {
                    if (item is JFCGridItem)
                    {
                        (item as JFCGridItem).ContextMenu = this.Parent.ContextmenuRow;
                    }
                    else if (item is JFCBorder)
                    {
                        JFCBorder jFCBorder = item as JFCBorder;
                        if (jFCBorder.Orientation == Orientation.Horizontal)
                        {
                            jFCBorder.ContextMenu = this.Parent.ContextmenuRow;
                        }
                    }
                    else if (item is HierarchicalItem)
                    {
                        HierarchicalItem obj = item as HierarchicalItem;
                        obj.ContextMenu = this.Parent.ContextmenuRow;
                        obj.Component.ContextMenu = this.Parent.ContextmenuRow;
                    }
                    else if (item is FrameworkElement)
                    {
                        (item as FrameworkElement).ContextMenu = this.Parent.ContextmenuRow;
                    }
                }
            }
        }

        private void ApplyStyleCell(CellImg Cell, JFCGridColumn Column)
        {
            this.ApplyStyleCell(Cell, Column, ApplyMode.View);
        }

        private void ApplyStyleCell(CellImg Cell, JFCGridColumn Column, ApplyMode Mode)
        {
            if (Mode == ApplyMode.View)
            {
                Cell.TextAlignment = Column.CellStyle.TextAlignment;
                Column.CellStyle.PropertyChanged += delegate
                {
                    Cell.TextAlignment = Column.CellStyle.TextAlignment;
                };
            }
            Cell.Margin = Column.CellPadding;
            if (Mode == ApplyMode.View)
            {
                Column.PropertyChanged += delegate (object sender, PropertyChangedEventArgs e)
                {
                    if (e.PropertyName == "Margin")
                    {
                        Cell.Margin = Column.CellPadding;
                    }
                };
            }
        }

        public void RefreshHorizontalBorderColor()
        {
            List<UIElement> list = (from UIElement ctrl in this.Grid10.Children
                                    where ctrl is JFCBorder
                                    select ctrl).ToList();
            List<UIElement>.Enumerator enumerator = list.GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    JFCBorder jFCBorder = enumerator.Current as JFCBorder;
                    if (jFCBorder.Orientation == Orientation.Horizontal)
                    {
                        jFCBorder.BorderBrush = this.Parent.HorizontalBorderColor;
                    }
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            (from UIElement ctrl in this.Grid20.Children
             where ctrl is JFCBorder
             select ctrl).ToList();
            enumerator = list.GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    JFCBorder jFCBorder2 = enumerator.Current as JFCBorder;
                    if (jFCBorder2.Orientation == Orientation.Horizontal)
                    {
                        jFCBorder2.BorderBrush = this.Parent.HorizontalBorderColor;
                    }
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            enumerator = (from UIElement ctrl in this.Grid11.Children
                          where ctrl is JFCBorder
                          select ctrl).ToList().GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    JFCBorder jFCBorder3 = enumerator.Current as JFCBorder;
                    if (jFCBorder3.Orientation == Orientation.Horizontal)
                    {
                        jFCBorder3.BorderBrush = this.Parent.HorizontalBorderColor;
                    }
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            enumerator = (from UIElement ctrl in this.Grid21.Children
                          where ctrl is JFCBorder
                          select ctrl).ToList().GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    JFCBorder jFCBorder4 = enumerator.Current as JFCBorder;
                    if (jFCBorder4.Orientation == Orientation.Horizontal)
                    {
                        jFCBorder4.BorderBrush = this.Parent.HorizontalBorderColor;
                    }
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            enumerator = (from UIElement ctrl in this.Grid12.Children
                          where ctrl is JFCBorder
                          select ctrl).ToList().GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    JFCBorder jFCBorder5 = enumerator.Current as JFCBorder;
                    if (jFCBorder5.Orientation == Orientation.Horizontal)
                    {
                        jFCBorder5.BorderBrush = this.Parent.HorizontalBorderColor;
                    }
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
        }

        public void RefreshVerticalBorderColor()
        {
            List<UIElement>.Enumerator enumerator = (from UIElement ctrl in this.Grid10.Children
                                                     where ctrl is JFCBorder
                                                     select ctrl).ToList().GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    JFCBorder jFCBorder = enumerator.Current as JFCBorder;
                    if (jFCBorder.Orientation == Orientation.Vertical)
                    {
                        jFCBorder.BorderBrush = this.Parent.VerticalBorderColor;
                    }
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            enumerator = (from UIElement ctrl in this.Grid20.Children
                          where ctrl is JFCBorder
                          select ctrl).ToList().GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    JFCBorder jFCBorder2 = enumerator.Current as JFCBorder;
                    if (jFCBorder2.Orientation == Orientation.Vertical)
                    {
                        jFCBorder2.BorderBrush = this.Parent.VerticalBorderColor;
                    }
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            enumerator = (from UIElement ctrl in this.Grid11.Children
                          where ctrl is JFCBorder
                          select ctrl).ToList().GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    JFCBorder jFCBorder3 = enumerator.Current as JFCBorder;
                    if (jFCBorder3.Orientation == Orientation.Vertical)
                    {
                        jFCBorder3.BorderBrush = this.Parent.VerticalBorderColor;
                    }
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
            enumerator = (from UIElement ctrl in this.Grid21.Children
                          where ctrl is JFCBorder
                          select ctrl).ToList().GetEnumerator();
            try
            {
                while (enumerator.MoveNext())
                {
                    JFCBorder jFCBorder4 = enumerator.Current as JFCBorder;
                    if (jFCBorder4.Orientation == Orientation.Vertical)
                    {
                        jFCBorder4.BorderBrush = this.Parent.VerticalBorderColor;
                    }
                }
            }
            finally
            {
                ((IDisposable)enumerator).Dispose();
            }
        }

        private void UpdateCellGroupingSizeAndVisibility(JFCGridCellGrouping cg, int indexData, JFCGridColumn column, JFCExpendItem datacontext)
        {
            if (datacontext != null)
            {
                int num = this.Parent.NbRowAffichable();
                int num2 = 0;
                num2 = datacontext.NbChildrenView - (indexData - datacontext.StartChildViewIndex);
                int num3 = (int)this.Parent.ScrollBarVertical.Value + num - 1;
                if (num2 + indexData - 1 > num3)
                {
                    if (num3 - indexData + 1 > 0)
                    {
                        cg.SetValue(Grid.RowSpanProperty, num3 - indexData + 1);
                    }
                    else
                    {
                        cg.SetValue(Grid.RowSpanProperty, 1);
                    }
                }
                else if (num2 > 0)
                {
                    cg.SetValue(Grid.RowSpanProperty, num2);
                }
                else
                {
                    cg.SetValue(Grid.RowSpanProperty, 1);
                }
                if (datacontext.StartChildViewIndex == indexData || this.Parent.ScrollVerticalValue == indexData)
                {
                    cg.Visibility = Visibility.Visible;
                }
                else
                {
                    cg.Visibility = Visibility.Hidden;
                }
            }
            else
            {
                cg.Visibility = Visibility.Hidden;
            }
        }

        private void SelectionSimple(JFCGridItem Item)
        {
            this.Parent.SelectedExpendItems.RemoveWhere((JFCExpendItem row) => true);
            this.Parent.SelectedExpendItems.Add(Item.Data);
            this.Parent.OnSelectedItemsChanged(this.Parent, new RoutedEventArgs());
        }

        private void SelectionSimple(IEnumerable<JFCExpendItem> lstItem)
        {
            this.Parent.SelectedExpendItems.RemoveWhere((JFCExpendItem row) => true);
            foreach (JFCExpendItem item in lstItem)
            {
                this.Parent.SelectedExpendItems.Add(item);
            }
            this.Parent.OnSelectedItemsChanged(this.Parent, new RoutedEventArgs());
        }

        private void SelectionMultiple(JFCGridItem Item)
        {
            if (this.Parent.SelectedExpendItems.Contains(Item.Data))
            {
                this.Parent.SelectedExpendItems.Remove(Item.Data);
            }
            else
            {
                this.Parent.SelectedExpendItems.Add(Item.Data);
            }
            this.Parent.OnSelectedItemsChanged(this.Parent, new RoutedEventArgs());
        }

        private void SelectionMultiple(IEnumerable<JFCExpendItem> lstItem)
        {
            foreach (JFCExpendItem item in lstItem)
            {
                if (this.Parent.SelectedExpendItems.Contains(item))
                {
                    this.Parent.SelectedExpendItems.Remove(item);
                }
                else
                {
                    this.Parent.SelectedExpendItems.Add(item);
                }
            }
            this.Parent.OnSelectedItemsChanged(this.Parent, new RoutedEventArgs());
        }

        private void SelectionExtendedControl(JFCGridItem Item)
        {
            if (this.Parent.SelectedExpendItems.Contains(Item.Data))
            {
                this.Parent.SelectedExpendItems.Remove(Item.Data);
            }
            else
            {
                this.Parent.SelectedExpendItems.Add(Item.Data);
            }
            this.Parent.OnSelectedItemsChanged(this.Parent, new RoutedEventArgs());
        }

        private void SelectionExtendedControl(IEnumerable<JFCExpendItem> lstItem)
        {
            foreach (JFCExpendItem item in lstItem)
            {
                if (this.Parent.SelectedExpendItems.Contains(item))
                {
                    this.Parent.SelectedExpendItems.Remove(item);
                }
                else
                {
                    this.Parent.SelectedExpendItems.Add(item);
                }
            }
            this.Parent.OnSelectedItemsChanged(this.Parent, new RoutedEventArgs());
        }

        private void SelectionExtendedShift(JFCGridItem Item)
        {
            if (this.Parent.SelectedExpendItems.Count == 0)
            {
                this.Parent.SelectedExpendItems.Add(this.Parent.dataSource[0]);
            }
            else
            {
                JFCExpendItem item = this.Parent.SelectedExpendItems.ElementAt(0);
                this.Parent.SelectedExpendItems.Clear();
                this.Parent.SelectedExpendItems.Add(item);
            }
            int num = 0;
            int num2 = 0;
            bool flag = false;
            bool flag2 = false;
            for (int i = 0; i < this.Parent.dataSource.Count(); i++)
            {
                if (this.Parent.dataSource[i] == this.Parent.SelectedExpendItems.ElementAt(0))
                {
                    num = i;
                    flag = true;
                }
                if (this.Parent.dataSource[i] == Item.Data)
                {
                    num2 = i;
                    flag2 = true;
                }
                if (flag & flag2)
                {
                    break;
                }
            }
            if (num <= num2)
            {
                for (int j = num + 1; j <= num2; j++)
                {
                    this.Parent.SelectedExpendItems.Add(this.Parent.dataSource[j]);
                }
            }
            else
            {
                for (int k = num2; k < num; k++)
                {
                    this.Parent.SelectedExpendItems.Add(this.Parent.dataSource[k]);
                }
            }
            this.Parent.OnSelectedItemsChanged(this.Parent, new RoutedEventArgs());
        }

        private void SelectionExtendedShift(IEnumerable<JFCExpendItem> lstItem)
        {
            if (this.Parent.SelectedExpendItems.Count == 0)
            {
                this.Parent.SelectedExpendItems.Add(this.Parent.dataSource[0]);
            }
            else
            {
                JFCExpendItem item = this.Parent.SelectedExpendItems.ElementAt(0);
                this.Parent.SelectedExpendItems.Clear();
                this.Parent.SelectedExpendItems.Add(item);
            }
            int num = 0;
            int num2 = 0;
            bool flag = false;
            bool flag2 = false;
            for (int i = 0; i < this.Parent.dataSource.Count(); i++)
            {
                if (this.Parent.dataSource[i] == this.Parent.SelectedExpendItems.ElementAt(0))
                {
                    num = i;
                    flag = true;
                }
                if (this.Parent.dataSource[i] == lstItem.ElementAt(0))
                {
                    num2 = i;
                    flag2 = true;
                }
                if (flag & flag2)
                {
                    break;
                }
            }
            if (num <= num2)
            {
                for (int j = num + 1; j <= num2; j++)
                {
                    this.Parent.SelectedExpendItems.Add(this.Parent.dataSource[j]);
                }
                foreach (JFCExpendItem item2 in lstItem)
                {
                    this.Parent.SelectedExpendItems.Add(item2);
                }
            }
            else
            {
                for (int k = num2; k < num; k++)
                {
                    this.Parent.SelectedExpendItems.Add(this.Parent.dataSource[k]);
                }
            }
            this.Parent.OnSelectedItemsChanged(this.Parent, new RoutedEventArgs());
        }

        public void SelectAll()
        {
            this.Parent.SelectedExpendItems.RemoveWhere((JFCExpendItem row) => true);
            JFCExpendItem[] dataSource = this.Parent.dataSource;
            foreach (JFCExpendItem item in dataSource)
            {
                this.Parent.SelectedExpendItems.Add(item);
            }
            this.RefreshSelection();
            this.Parent.OnSelectedItemsChanged(this.Parent, new RoutedEventArgs());
        }

        public void RefreshSelection()
        {
            foreach (UIElement item in from UIElement ctrl in this.Grid11.Children
                                       where ctrl is JFCGridItem
                                       select ctrl)
            {
                JFCGridItem jFCGridItem = item as JFCGridItem;
                if (this.Parent.SelectedExpendItems.Contains(jFCGridItem.Data) || this.Parent.SelectedExpendItemsChildren.Contains(jFCGridItem.Data))
                {
                    if (!jFCGridItem.IsSelected)
                    {
                        jFCGridItem.IsSelected = true;
                        jFCGridItem.SelectedPeer(true);
                    }
                }
                else if (jFCGridItem.IsSelected)
                {
                    jFCGridItem.IsSelected = false;
                    jFCGridItem.SelectedPeer(false);
                }
            }
            IEnumerable<UIElement> first = from UIElement ctrl in this.Grid10.Children
                                           where ctrl is JFCGridCellGrouping
                                           select ctrl;
            IEnumerable<UIElement> second = from UIElement ctrl in this.Grid11.Children
                                            where ctrl is JFCGridCellGrouping
                                            select ctrl;
            IEnumerable<UIElement> second2 = from UIElement ctrl in this.Grid12.Children
                                             where ctrl is JFCGridCellGrouping
                                             select ctrl;
            foreach (UIElement item2 in first.Concat(second).Concat(second2))
            {
                JFCGridCellGrouping jFCGridCellGrouping = item2 as JFCGridCellGrouping;
                JFCExpendItem jFCExpendItem = jFCGridCellGrouping.DataContext as JFCExpendItem;
                if (jFCExpendItem != null)
                {
                    if (this.Parent.SelectedExpendItems.Contains(jFCExpendItem) || this.Parent.SelectedExpendItemsChildren.Contains(jFCExpendItem))
                    {
                        if (!jFCGridCellGrouping.IsSelected)
                        {
                            jFCGridCellGrouping.IsSelected = true;
                        }
                    }
                    else if (jFCGridCellGrouping.IsSelected)
                    {
                        jFCGridCellGrouping.IsSelected = false;
                    }
                }
            }
        }

        private void item_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            JFCGridItem jFCGridItem = sender as JFCGridItem;
            if (this.Parent.SelectionMode == SelectionMode.Single)
            {
                this.SelectionSimple(jFCGridItem);
                this.RefreshSelection();
            }
            else if (this.Parent.SelectionMode == SelectionMode.Multiple)
            {
                this.SelectionMultiple(jFCGridItem);
                this.RefreshSelection();
            }
            else if (this.Parent.SelectionMode == SelectionMode.Extended)
            {
                if (Keyboard.IsKeyDown(Key.RightShift) || Keyboard.IsKeyDown(Key.LeftShift))
                {
                    this.SelectionExtendedShift(jFCGridItem);
                }
                else if (!Keyboard.IsKeyDown(Key.RightCtrl) && !Keyboard.IsKeyDown(Key.LeftCtrl) && !this.Parent.SelectedExpendItems.Contains(jFCGridItem.Data))
                {
                    this.SelectionSimple(jFCGridItem);
                }
                this.RefreshSelection();
            }
        }

        private void item_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            JFCGridItem item = sender as JFCGridItem;
            if (this.Parent.SelectionMode == SelectionMode.Extended)
            {
                if (Keyboard.IsKeyDown(Key.RightCtrl) || Keyboard.IsKeyDown(Key.LeftCtrl))
                {
                    this.SelectionExtendedControl(item);
                }
                else if (!Keyboard.IsKeyDown(Key.RightShift) && !Keyboard.IsKeyDown(Key.LeftShift))
                {
                    this.SelectionSimple(item);
                }
                this.RefreshSelection();
            }
        }

        private void item_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            JFCGridItem jFCGridItem = sender as JFCGridItem;
            if (!this.Parent.SelectedExpendItems.Contains(jFCGridItem.Data))
            {
                this.SelectionSimple(jFCGridItem);
                this.RefreshSelection();
            }
        }

        private void item_IsMouseOverLineChanged(object sender, RoutedEventArgs e)
        {
            JFCGridItem jFCGridItem = sender as JFCGridItem;
            if (jFCGridItem.Parent != null)
            {
                JFCGrid.DependencyPropertyLineOverChangedEventArgs.GridPosition position = (JFCGrid.DependencyPropertyLineOverChangedEventArgs.GridPosition)((jFCGridItem.Parent != this.Grid10) ? ((jFCGridItem.Parent == this.Grid11) ? 1 : ((jFCGridItem.Parent != this.Grid20) ? ((jFCGridItem.Parent != this.Grid21) ? 4 : 3) : 2)) : 0);
                if (jFCGridItem.IsMouseOverLine)
                {
                    if (jFCGridItem.Data != null)
                    {
                        this.Parent.OnLineOverChanged(this, new JFCGrid.DependencyPropertyLineOverChangedEventArgs(FrameworkElement.DataContextProperty, null, jFCGridItem.Data.Obj, position, jFCGridItem.IndexData));
                    }
                }
                else if (jFCGridItem.Data != null)
                {
                    this.Parent.OnLineOverChanged(this, new JFCGrid.DependencyPropertyLineOverChangedEventArgs(FrameworkElement.DataContextProperty, jFCGridItem.Data.Obj, null, position, jFCGridItem.IndexData));
                }
            }
        }
    }
}