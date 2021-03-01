// Facility_Management.My.MyProject
using System;
using System.CodeDom.Compiler;
using System.Collections;
using System.ComponentModel;
using System.ComponentModel.Design;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using Facility_Management;
using Facility_Management.My;
using Microsoft.VisualBasic;
using Microsoft.VisualBasic.ApplicationServices;
using Microsoft.VisualBasic.CompilerServices;

namespace Facility_Management.My
{
    [StandardModule]
    [HideModuleName]
    [GeneratedCode("MyTemplate", "11.0.0.0")]
    internal sealed class MyProject
    {
        [EditorBrowsable(EditorBrowsableState.Never)]
        [MyGroupCollection("System.Windows.Forms.Form", "Create__Instance__", "Dispose__Instance__", "My.MyProject.Forms")]
        internal sealed class MyForms
        {
            [ThreadStatic]
            private static Hashtable m_FormBeingCreated;

            [EditorBrowsable(EditorBrowsableState.Never)]
            public CreateUpdate m_CreateUpdate;

            [EditorBrowsable(EditorBrowsableState.Never)]
            public DataRecovery m_DataRecovery;

            [EditorBrowsable(EditorBrowsableState.Never)]
            public DataRecoveryP m_DataRecoveryP;

            [EditorBrowsable(EditorBrowsableState.Never)]
            public FormBox m_FormBox;

            [EditorBrowsable(EditorBrowsableState.Never)]
            public GetElements m_GetElements;

            [EditorBrowsable(EditorBrowsableState.Never)]
            public PleaseWait m_PleaseWait;

            public CreateUpdate CreateUpdate
            {
                get
                {
                    m_CreateUpdate = Create__Instance__(m_CreateUpdate);
                    return m_CreateUpdate;
                }
                set
                {
                    if (value != m_CreateUpdate)
                    {
                        if (value != null)
                        {
                            throw new ArgumentException("Property can only be set to Nothing");
                        }
                        Dispose__Instance__(ref m_CreateUpdate);
                    }
                }
            }

            public DataRecovery DataRecovery
            {
                get
                {
                    m_DataRecovery = Create__Instance__(m_DataRecovery);
                    return m_DataRecovery;
                }
                set
                {
                    if (value != m_DataRecovery)
                    {
                        if (value != null)
                        {
                            throw new ArgumentException("Property can only be set to Nothing");
                        }
                        Dispose__Instance__(ref m_DataRecovery);
                    }
                }
            }

            public DataRecoveryP DataRecoveryP
            {
                get
                {
                    m_DataRecoveryP = Create__Instance__(m_DataRecoveryP);
                    return m_DataRecoveryP;
                }
                set
                {
                    if (value != m_DataRecoveryP)
                    {
                        if (value != null)
                        {
                            throw new ArgumentException("Property can only be set to Nothing");
                        }
                        Dispose__Instance__(ref m_DataRecoveryP);
                    }
                }
            }

            public FormBox FormBox
            {
                get
                {
                    m_FormBox = Create__Instance__(m_FormBox);
                    return m_FormBox;
                }
                set
                {
                    if (value != m_FormBox)
                    {
                        if (value != null)
                        {
                            throw new ArgumentException("Property can only be set to Nothing");
                        }
                        Dispose__Instance__(ref m_FormBox);
                    }
                }
            }

            public GetElements GetElements
            {
                get
                {
                    m_GetElements = Create__Instance__(m_GetElements);
                    return m_GetElements;
                }
                set
                {
                    if (value != m_GetElements)
                    {
                        if (value != null)
                        {
                            throw new ArgumentException("Property can only be set to Nothing");
                        }
                        Dispose__Instance__(ref m_GetElements);
                    }
                }
            }

            public PleaseWait PleaseWait
            {
                get
                {
                    m_PleaseWait = Create__Instance__(m_PleaseWait);
                    return m_PleaseWait;
                }
                set
                {
                    if (value != m_PleaseWait)
                    {
                        if (value != null)
                        {
                            throw new ArgumentException("Property can only be set to Nothing");
                        }
                        Dispose__Instance__(ref m_PleaseWait);
                    }
                }
            }

            [DebuggerHidden]
            private static T Create__Instance__<T>(T Instance) where T : Form, new()
            {
                if (Instance == null || Instance.IsDisposed)
                {
                    if (m_FormBeingCreated != null)
                    {
                        if (m_FormBeingCreated.ContainsKey(typeof(T)))
                        {
                            throw new InvalidOperationException(Utils.GetResourceString("WinForms_RecursiveFormCreate"));
                        }
                    }
                    else
                    {
                        m_FormBeingCreated = new Hashtable();
                    }
                    m_FormBeingCreated.Add(typeof(T), null);
                    TargetInvocationException ex2 = default(TargetInvocationException);
                    try
                    {
                        return new T();
                    }
                    catch (TargetInvocationException ex) when (((Func<bool>)delegate
                    {
                        // Could not convert BlockContainer to single expression
                        ProjectData.SetProjectError(ex);
                        ex2 = ex;
                        return ex2.InnerException != null;
                    }).Invoke())
                    {
                        throw new InvalidOperationException(Utils.GetResourceString("WinForms_SeeInnerException", ex2.InnerException.Message), ex2.InnerException);
                    }
                    finally
                    {
                        m_FormBeingCreated.Remove(typeof(T));
                    }
                }
                return Instance;
            }

            [DebuggerHidden]
            private void Dispose__Instance__<T>(ref T instance) where T : Form
            {
                instance.Dispose();
                instance = null;
            }

            [DebuggerHidden]
            [EditorBrowsable(EditorBrowsableState.Never)]
            public MyForms()
            {
            }

            [EditorBrowsable(EditorBrowsableState.Never)]
            public override bool Equals(object o)
            {
                return base.Equals(RuntimeHelpers.GetObjectValue(o));
            }

            [EditorBrowsable(EditorBrowsableState.Never)]
            public override int GetHashCode()
            {
                return base.GetHashCode();
            }

            [EditorBrowsable(EditorBrowsableState.Never)]
            internal new Type GetType()
            {
                return typeof(MyForms);
            }

            [EditorBrowsable(EditorBrowsableState.Never)]
            public override string ToString()
            {
                return base.ToString();
            }
        }

        [EditorBrowsable(EditorBrowsableState.Never)]
        [MyGroupCollection("System.Web.Services.Protocols.SoapHttpClientProtocol", "Create__Instance__", "Dispose__Instance__", "")]
        internal sealed class MyWebServices
        {
            [EditorBrowsable(EditorBrowsableState.Never)]
            [DebuggerHidden]
            public override bool Equals(object o)
            {
                return base.Equals(RuntimeHelpers.GetObjectValue(o));
            }

            [EditorBrowsable(EditorBrowsableState.Never)]
            [DebuggerHidden]
            public override int GetHashCode()
            {
                return base.GetHashCode();
            }

            [EditorBrowsable(EditorBrowsableState.Never)]
            [DebuggerHidden]
            internal new Type GetType()
            {
                return typeof(MyWebServices);
            }

            [EditorBrowsable(EditorBrowsableState.Never)]
            [DebuggerHidden]
            public override string ToString()
            {
                return base.ToString();
            }

            [DebuggerHidden]
            private static T Create__Instance__<T>(T instance) where T : new()
            {
                if (instance == null)
                {
                    return new T();
                }
                return instance;
            }

            [DebuggerHidden]
            private void Dispose__Instance__<T>(ref T instance)
            {
                instance = default(T);
            }

            [DebuggerHidden]
            [EditorBrowsable(EditorBrowsableState.Never)]
            public MyWebServices()
            {
            }
        }

        [EditorBrowsable(EditorBrowsableState.Never)]
        [ComVisible(false)]
        internal sealed class ThreadSafeObjectProvider<T> where T : new()
        {
            [CompilerGenerated]
            [ThreadStatic]
            private static T m_ThreadStaticValue;

            internal T GetInstance
            {
                [DebuggerHidden]
                get
                {
                    if (m_ThreadStaticValue == null)
                    {
                        m_ThreadStaticValue = new T();
                    }
                    return m_ThreadStaticValue;
                }
            }

            [DebuggerHidden]
            [EditorBrowsable(EditorBrowsableState.Never)]
            public ThreadSafeObjectProvider()
            {
            }
        }

        private static readonly ThreadSafeObjectProvider<MyComputer> m_ComputerObjectProvider = new ThreadSafeObjectProvider<MyComputer>();

        private static readonly ThreadSafeObjectProvider<MyApplication> m_AppObjectProvider = new ThreadSafeObjectProvider<MyApplication>();

        private static readonly ThreadSafeObjectProvider<User> m_UserObjectProvider = new ThreadSafeObjectProvider<User>();

        private static ThreadSafeObjectProvider<MyForms> m_MyFormsObjectProvider = new ThreadSafeObjectProvider<MyForms>();

        private static readonly ThreadSafeObjectProvider<MyWebServices> m_MyWebServicesObjectProvider = new ThreadSafeObjectProvider<MyWebServices>();

        [HelpKeyword("My.Computer")]
        internal static MyComputer Computer
        {
            [DebuggerHidden]
            get
            {
                return m_ComputerObjectProvider.GetInstance;
            }
        }

        [HelpKeyword("My.Application")]
        internal static MyApplication Application
        {
            [DebuggerHidden]
            get
            {
                return m_AppObjectProvider.GetInstance;
            }
        }

        [HelpKeyword("My.User")]
        internal static User User
        {
            [DebuggerHidden]
            get
            {
                return m_UserObjectProvider.GetInstance;
            }
        }

        [HelpKeyword("My.Forms")]
        internal static MyForms Forms
        {
            [DebuggerHidden]
            get
            {
                return m_MyFormsObjectProvider.GetInstance;
            }
        }

        [HelpKeyword("My.WebServices")]
        internal static MyWebServices WebServices
        {
            [DebuggerHidden]
            get
            {
                return m_MyWebServicesObjectProvider.GetInstance;
            }
        }
    }
}
