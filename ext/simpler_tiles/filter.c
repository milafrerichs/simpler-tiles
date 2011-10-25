#include "filter.h"
#include <simple-tiles/filter.h>

VALUE cSimplerTilesFilter;

static simplet_filter_t *
get_filter(VALUE self){
  simplet_filter_t *filter;
  Data_Get_Struct(self, simplet_filter_t, filter);
  return filter;
}

static VALUE
set_query(VALUE self, VALUE query){
  Check_Type(query, T_STRING);
  simplet_filter_t *filter = get_filter(self);
  simplet_filter_set_query(filter, RSTRING_PTR(query));
  return query;
}

static VALUE
get_query(VALUE self){
  simplet_filter_t *filter = get_filter(self);
  char *str;
  simplet_filter_get_query(filter, &str);
  return rb_str_new2(str);
}

// TODO: return newly created style
static VALUE
add_style(VALUE self, VALUE key, VALUE arg){
  Check_Type(key, T_STRING);
  Check_Type(arg, T_STRING);
  simplet_filter_t *filter = get_filter(self);
  simplet_filter_add_style(filter, RSTRING_PTR(key), RSTRING_PTR(arg));
  return Qnil;
}

static VALUE
filter_alloc(VALUE klass){
  simplet_filter_t *filter;

  if(!(filter = simplet_filter_new(NULL)))
    rb_fatal("Could not allocate space for a new SimplerTiles::Filter in memory.");

  return Data_Wrap_Struct(klass, NULL, simplet_filter_free, filter);
};


void init_filter(){
  VALUE rfilter = rb_define_class_under(mSimplerTiles, "Filter", rb_cObject);
  rb_define_alloc_func(rfilter, filter_alloc);
  rb_define_method(rfilter, "query=", set_query, 1);
  rb_define_method(rfilter, "query", get_query, 0);
  rb_define_private_method(rfilter, "add_style", add_style, 2);

  cSimplerTilesFilter = rfilter;
}