/* dbus.c generated by valac, the Vala compiler
 * generated from dbus.vala, do not modify */

#ifdef HAVE_CONFIG_H
#include  <config.h>
#else
#error <please run configure>
#endif

#include <glib.h>
#include <glib-object.h>
#include <dbus/dbus-glib-lowlevel.h>
#include <dbus/dbus-glib.h>

#define TYPE_RUIJIE_DBUS (ruijie_dbus_get_type ())
#define RUIJIE_DBUS(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_RUIJIE_DBUS, RuijieDbus))
#define RUIJIE_DBUS_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_RUIJIE_DBUS, RuijieDbusClass))
#define IS_RUIJIE_DBUS(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_RUIJIE_DBUS))
#define IS_RUIJIE_DBUS_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_RUIJIE_DBUS))
#define RUIJIE_DBUS_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_RUIJIE_DBUS, RuijieDbusClass))

typedef struct _RuijieDbus RuijieDbus;
typedef struct _RuijieDbusClass RuijieDbusClass;
typedef struct _RuijieDbusPrivate RuijieDbusPrivate;
#define _dbus_g_connection_unref0(var) ((var == NULL) ? NULL : (var = (dbus_g_connection_unref (var), NULL)))
#define _g_object_unref0(var) ((var == NULL) ? NULL : (var = (g_object_unref (var), NULL)))
typedef struct _Block1Data Block1Data;
#define _g_main_loop_unref0(var) ((var == NULL) ? NULL : (var = (g_main_loop_unref (var), NULL)))
#define _g_error_free0(var) ((var == NULL) ? NULL : (var = (g_error_free (var), NULL)))

struct _RuijieDbus
{
  GObject parent_instance;
  RuijieDbusPrivate * priv;
};

struct _RuijieDbusClass
{
  GObjectClass parent_class;
};

struct _RuijieDbusPrivate
{
  DBusGConnection* conn;
  DBusGProxy* networkmanager;
};

typedef gint
(*Fuction)(void* user_data);
struct _Block1Data
{
  int _ref_count_;
  Fuction fast;
  gpointer fast_target;
};

static gpointer ruijie_dbus_parent_class = NULL;
extern RuijieDbus* dbus;
RuijieDbus* dbus = NULL;
extern GMainLoop* loop;
GMainLoop* loop = NULL;

GType
ruijie_dbus_get_type(void);
#define RUIJIE_DBUS_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), TYPE_RUIJIE_DBUS, RuijieDbusPrivate))
enum
{
  RUIJIE_DBUS_DUMMY_PROPERTY
};
static void
ruijie_dbus_sig_StateChanged(RuijieDbus* self, DBusGProxy* obj_, guint32 i);
static void
_ruijie_dbus_sig_StateChanged_dynamic_StateChanged0_(DBusGProxy* _sender,
    guint32 i, gpointer self);
void
_dynamic_StateChanged1_connect(gpointer obj, const char * signal_name,
    GCallback handler, gpointer data);
void
ruijie_dbus_conn_dbus(RuijieDbus* self, GError** error);
static inline guint32
_dynamic_get_State0(DBusGProxy* obj);
guint32
ruijie_dbus_get_state(RuijieDbus* self);
RuijieDbus*
ruijie_dbus_new(void);
RuijieDbus*
ruijie_dbus_construct(GType object_type);
static void
ruijie_dbus_finalize(GObject* obj);
static void
_lambda0_(Block1Data* _data1_);
static void
__lambda0__ruijie_dbus_sig_p_statechanged(RuijieDbus* _sender, gpointer self);
static Block1Data*
block1_data_ref(Block1Data* _data1_);
static void
block1_data_unref(Block1Data* _data1_);
gint
connect_to_sig_StateChanged(Fuction fast, void* fast_target);
gint
g_loop_run(void);
gint
g_loop_quit(void);
gint
dbus_init(void);
gboolean
is_networ_ready(void);

static void
_ruijie_dbus_sig_StateChanged_dynamic_StateChanged0_(DBusGProxy* _sender,
    guint32 i, gpointer self)
{
  ruijie_dbus_sig_StateChanged(self, _sender, i);
}

void
_dynamic_StateChanged1_connect(gpointer obj, const char * signal_name,
    GCallback handler, gpointer data)
{
  dbus_g_object_register_marshaller(g_cclosure_marshal_VOID__UINT, G_TYPE_NONE,
      G_TYPE_UINT, G_TYPE_INVALID);
  dbus_g_proxy_add_signal(obj, "StateChanged", G_TYPE_UINT, G_TYPE_INVALID);
  dbus_g_proxy_connect_signal(obj, signal_name, handler, data, NULL);
}

void
ruijie_dbus_conn_dbus(RuijieDbus* self, GError** error)
{
  GError * _inner_error_;
  DBusGConnection* _tmp0_;
  DBusGConnection* _tmp1_;
  DBusGProxy* _tmp2_;
  g_return_if_fail(self != NULL);
  _inner_error_ = NULL;
  _tmp0_ = dbus_g_bus_get(DBUS_BUS_SYSTEM, &_inner_error_);
  if (_inner_error_ != NULL)
    {
      g_propagate_error(error, _inner_error_);
      return;
    }
  self->priv->conn
      = (_tmp1_ = _tmp0_, _dbus_g_connection_unref0 (self->priv->conn), _tmp1_);
  self->priv->networkmanager
      = (_tmp2_ = dbus_g_proxy_new_for_name(self->priv->conn,
          "org.freedesktop.NetworkManager", "/org/freedesktop/NetworkManager",
          "org.freedesktop.NetworkManager"), _g_object_unref0 (self->priv->networkmanager), _tmp2_);
  _dynamic_StateChanged1_connect(self->priv->networkmanager, "StateChanged",
      (GCallback) _ruijie_dbus_sig_StateChanged_dynamic_StateChanged0_, self);
}

static inline guint32
_dynamic_get_State0(DBusGProxy* obj)
{
  DBusGProxy* property_proxy;
  GValue gvalue =
    { 0 };
  guint32 result;
  property_proxy = dbus_g_proxy_new_from_proxy(obj, DBUS_INTERFACE_PROPERTIES,
      NULL);
  dbus_g_proxy_call(property_proxy, "Get", NULL, G_TYPE_STRING,
      dbus_g_proxy_get_interface(obj), G_TYPE_STRING, "State", G_TYPE_INVALID,
      G_TYPE_VALUE, &gvalue, G_TYPE_INVALID);
  g_object_unref(property_proxy);
  result = g_value_get_uint(&gvalue);
  return result;
}

guint32
ruijie_dbus_get_state(RuijieDbus* self)
{
  guint32 result;
  g_return_val_if_fail(self != NULL, 0U);
  result = _dynamic_get_State0(self->priv->networkmanager);
  return result;
}

static void
ruijie_dbus_sig_StateChanged(RuijieDbus* self, DBusGProxy* obj_, guint32 i)
{
  g_return_if_fail(self != NULL);
  g_return_if_fail(obj_ != NULL);
  g_signal_emit_by_name(self, "sig-p-statechanged");
}

RuijieDbus*
ruijie_dbus_construct(GType object_type)
{
  RuijieDbus * self;
  self = (RuijieDbus*) g_object_new(object_type, NULL);
  return self;
}

RuijieDbus*
ruijie_dbus_new(void)
{
  return ruijie_dbus_construct(TYPE_RUIJIE_DBUS);
}

static void
ruijie_dbus_class_init(RuijieDbusClass * klass)
{
  ruijie_dbus_parent_class = g_type_class_peek_parent(klass);
  g_type_class_add_private(klass, sizeof(RuijieDbusPrivate));
  G_OBJECT_CLASS(klass)->finalize = ruijie_dbus_finalize;
  g_signal_new("sig_p_statechanged", TYPE_RUIJIE_DBUS, G_SIGNAL_RUN_LAST, 0,
      NULL, NULL, g_cclosure_marshal_VOID__VOID, G_TYPE_NONE, 0);
}

static void
ruijie_dbus_instance_init(RuijieDbus * self)
{
  self->priv = RUIJIE_DBUS_GET_PRIVATE (self);
}

static void
ruijie_dbus_finalize(GObject* obj)
{
  RuijieDbus * self;
  self = RUIJIE_DBUS (obj);
  _dbus_g_connection_unref0 (self->priv->conn);
  _g_object_unref0 (self->priv->networkmanager);
  G_OBJECT_CLASS(ruijie_dbus_parent_class)->finalize(obj);
}

GType
ruijie_dbus_get_type(void)
{
  static GType ruijie_dbus_type_id = 0;
  if (ruijie_dbus_type_id == 0)
    {
      static const GTypeInfo g_define_type_info =
        { sizeof(RuijieDbusClass), (GBaseInitFunc) NULL,
            (GBaseFinalizeFunc) NULL, (GClassInitFunc) ruijie_dbus_class_init,
            (GClassFinalizeFunc) NULL, NULL, sizeof(RuijieDbus), 0,
            (GInstanceInitFunc) ruijie_dbus_instance_init, NULL };
      ruijie_dbus_type_id = g_type_register_static(G_TYPE_OBJECT, "RuijieDbus",
          &g_define_type_info, 0);
    }
  return ruijie_dbus_type_id;
}

static void
_lambda0_(Block1Data* _data1_)
{
  _data1_->fast(_data1_->fast_target);
}

static void
__lambda0__ruijie_dbus_sig_p_statechanged(RuijieDbus* _sender, gpointer self)
{
  _lambda0_(self);
}

static Block1Data*
block1_data_ref(Block1Data* _data1_)
{
  ++_data1_->_ref_count_;
  return _data1_;
}

static void
block1_data_unref(Block1Data* _data1_)
{
  if ((--_data1_->_ref_count_) == 0)
    {
      g_slice_free(Block1Data, _data1_);
    }
}

gint
connect_to_sig_StateChanged(Fuction fast, void* fast_target)
{
  gint result;
  Block1Data* _data1_;
  _data1_ = g_slice_new0(Block1Data);
  _data1_->_ref_count_ = 1;
  _data1_->fast = fast;
  _data1_->fast_target = fast_target;
  if (dbus == NULL)
    {
      result = 1;
      block1_data_unref(_data1_);
      return result;
    }
  g_signal_connect_data(dbus, "sig-p-statechanged",
      (GCallback) __lambda0__ruijie_dbus_sig_p_statechanged, block1_data_ref(
          _data1_), (GClosureNotify) block1_data_unref, 0);
  result = 0;
  block1_data_unref(_data1_);
  return result;
}

gint
g_loop_run(void)
{
  gint result;
  if (loop == NULL)
    {
      GMainLoop* _tmp0_;
      loop
          = (_tmp0_ = g_main_loop_new(NULL, FALSE), _g_main_loop_unref0 (loop), _tmp0_);
    }
  g_main_loop_run(loop);
  result = 0;
  return result;
}

gint
g_loop_quit(void)
{
  gint result;
  if (loop == NULL)
    {
      result = 1;
      return result;
    }
  g_main_loop_quit(loop);
  result = 0;
  return result;
}

gint
dbus_init(void)
{
  gint result;
  GError * _inner_error_;
  RuijieDbus* _tmp0_;
  _inner_error_ = NULL;
  if (dbus != NULL)
    {
      result = 1;
      return result;
    }
  dbus = (_tmp0_ = ruijie_dbus_new(), _g_object_unref0 (dbus), _tmp0_);
    {
      ruijie_dbus_conn_dbus(dbus, &_inner_error_);
      if (_inner_error_ != NULL)
        {
          if (_inner_error_->domain == DBUS_GERROR)
            {
              goto __catch0_dbus_gerror;
            }
          goto __catch0_g_error;
          goto __finally0;
        }
    }
  goto __finally0;
  __catch0_dbus_gerror:
    {
      GError * e;
      e = _inner_error_;
      _inner_error_ = NULL;
        {
          g_warning("dbus.vala:63: Failed to initialise");
          result = 2;
          _g_error_free0 (e);
          return result;
        }
    }
  goto __finally0;
  __catch0_g_error:
    {
      GError * e;
      e = _inner_error_;
      _inner_error_ = NULL;
        {
          g_warning("dbus.vala:66: Dynamic method failure");
          result = 3;
          _g_error_free0 (e);
          return result;
        }
    }
  __finally0: if (_inner_error_ != NULL)
    {
      g_critical("file %s: line %d: uncaught error: %s", __FILE__, __LINE__,
          _inner_error_->message);
      g_clear_error(&_inner_error_);
      return 0;
    }
  result = 0;
  return result;
}

gboolean
is_networ_ready(void)
{
  gboolean result;
  if (dbus == NULL)
    {
      result = FALSE;
      return result;
    }
  if (ruijie_dbus_get_state(dbus) == 3)
    {
      result = TRUE;
      return result;
    }
  result = FALSE;
  return result;
}
