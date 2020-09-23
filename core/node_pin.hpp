//  This file is distributed under the BSD 3-Clause License. See LICENSE for details.
#pragma once

class LGraph;
class XEdge;
class Node;

#include <vector>

#include "ann_ssa.hpp"
#include "lgedge.hpp"
#include "mmap_map.hpp"
#include "hierarchy.hpp"

using XEdge_iterator    = std::vector<XEdge>;
using Node_pin_iterator = std::vector<Node_pin>;

class Node_pin {
protected:
  friend class LGraph;
  friend class LGraph_Node_Type;
  friend class XEdge;
  friend class Node;
  friend class Fast_edge_iterator;
  friend class Flow_base_iterator;
  friend class Fwd_edge_iterator;
  friend class Bwd_edge_iterator;
  friend class Edge_raw;

  LGraph *        top_g;
  LGraph *        current_g;
  Hierarchy_index hidx;
  Index_ID        idx;
  Port_ID         pid;
  bool            sink;

  constexpr Node_pin(LGraph *_g, LGraph *_c_g, const Hierarchy_index &_hidx, Index_ID _idx, Port_ID _pid, bool _sink)
    : top_g(_g), current_g(_c_g), hidx(_hidx), idx(_idx), pid(_pid), sink(_sink) {
    assert(_g);
    assert(_idx);
  }

  const Index_ID get_idx() const {
    I(idx);
    return idx;
  }
  const Index_ID get_root_idx() const;

public:
  class __attribute__((packed)) Compact {
  protected:
    Hierarchy_index hidx;
    uint32_t        idx : Index_bits;
    uint32_t        sink : 1;

    friend class LGraph;
    friend class LGraph_Node_Type;
    friend class Node;
    friend class Node_pin;
    friend class XEdge;
    friend class Fast_edge_iterator;
    friend class Flow_base_iterator;
    friend class Fwd_edge_iterator;
    friend class Bwd_edge_iterator;
    friend class mmap_lib::hash<Node_pin::Compact>;

  public:
    // constexpr operator size_t() const { I(0); return idx|(sink<<31); }

    Compact(const Compact &obj) : hidx(obj.hidx), idx(obj.idx), sink(obj.sink) {}
    Compact(const Hierarchy_index _hidx, Index_ID _idx, bool _sink) : hidx(_hidx), idx(_idx), sink(_sink){ I(!hidx.is_invalid()); };
    Compact() : idx(0), sink(0){};
    Compact &operator=(const Compact &obj) {
      I(this != &obj);
      hidx = obj.hidx;
      idx  = obj.idx;
      sink = obj.sink;

      return *this;
    };

    constexpr bool is_invalid() const { return idx == 0; }

    constexpr bool operator==(const Compact &other) const {
      return idx == other.idx
             && sink == other.sink
             && (hidx == other.hidx || hidx.is_invalid() || other.hidx.is_invalid());
    }
    constexpr bool operator!=(const Compact &other) const { return !(*this == other); }

    template <typename H>
    friend H AbslHashValue(H h, const Compact &s) {
      return H::combine(std::move(h), s.hidx.get_hash(), s.idx, s.sink);
    };
  };
  class __attribute__((packed)) Compact_driver {
  protected:
    Hierarchy_index hidx;
    uint32_t        idx : Index_bits;

    friend class LGraph;
    friend class LGraph_Node_Type;
    friend class Node;
    friend class Node_pin;
    friend class XEdge;
    friend class Fast_edge_iterator;
    friend class Flow_base_iterator;
    friend class Fwd_edge_iterator;
    friend class Bwd_edge_iterator;
    friend class mmap_lib::hash<Node_pin::Compact_driver>;

  public:
    // constexpr operator size_t() const { I(0); return idx|(sink<<31); }

    Compact_driver(const Compact_driver &obj) : hidx(obj.hidx), idx(obj.idx) {}
    Compact_driver(const Hierarchy_index _hidx, Index_ID _idx) : hidx(_hidx), idx(_idx){ I(!hidx.is_invalid()); };
    Compact_driver() : idx(0){};
    Compact_driver &operator=(const Compact_driver &obj) {
      I(this != &obj);
      hidx = obj.hidx;
      idx  = obj.idx;

      return *this;
    };

    constexpr bool is_invalid() const { return idx == 0; }

    constexpr bool operator==(const Compact_driver &other) const { 
      return idx == other.idx
             && (hidx == other.hidx || hidx.is_invalid() || other.hidx.is_invalid());
    }
    constexpr bool operator!=(const Compact_driver &other) const { return !(*this == other); }

    template <typename H>
    friend H AbslHashValue(H h, const Compact_driver &s) {
      return H::combine(std::move(h), s.hidx.get_hash(), s.idx);
    };
  };
  class __attribute__((packed)) Compact_class {
  protected:
    uint32_t idx : Index_bits;
    uint32_t sink : 1;

    friend class LGraph;
    friend class LGraph_Node_Type;
    friend class Node;
    friend class Node_pin;
    friend class XEdge;
    friend class Fast_edge_iterator;
    friend class Flow_base_iterator;
    friend class Fwd_edge_iterator;
    friend class Bwd_edge_iterator;
    friend class mmap_lib::hash<Node_pin::Compact_class>;

  public:
    // constexpr operator size_t() const { I(0); return idx|(sink<<31); }

    Compact_class(const Compact_class &obj) : idx(obj.idx), sink(obj.sink) {}
    Compact_class(Index_ID _idx, bool _sink) : idx(_idx), sink(_sink) {}
    Compact_class() : idx(0), sink(0) {}
    Compact_class &operator=(const Compact_class &obj) {
      I(this != &obj);
      idx  = obj.idx;
      sink = obj.sink;

      return *this;
    }

    constexpr bool is_invalid() const { return idx == 0; }

    constexpr bool operator==(const Compact_class &other) const { return idx == other.idx && sink == other.sink; }
    constexpr bool operator!=(const Compact_class &other) const { return !(*this == other); }

    template <typename H>
    friend H AbslHashValue(H h, const Compact_class &s) {
      return H::combine(std::move(h), s.idx, s.sink);
    }
  };
  class __attribute__((packed)) Compact_class_driver {
  protected:
    uint32_t idx : Index_bits;

    friend class LGraph;
    friend class LGraph_Node_Type;
    friend class Node;
    friend class Node_pin;
    friend class XEdge;
    friend class Fast_edge_iterator;
    friend class Flow_base_iterator;
    friend class Fwd_edge_iterator;
    friend class Bwd_edge_iterator;
    friend class mmap_lib::hash<Node_pin::Compact_class_driver>;

  public:
    // constexpr operator size_t() const { I(0); return idx|(sink<<31); }

    Compact_class_driver(const Compact_class_driver &obj) : idx(obj.idx) {}
    Compact_class_driver(Index_ID _idx) : idx(_idx) {}
    Compact_class_driver() : idx(0) {}
    Compact_class_driver &operator=(const Compact_class_driver &obj) {
      I(this != &obj);
      idx = obj.idx;

      return *this;
    }

    constexpr bool is_invalid() const { return idx == 0; }

    constexpr bool operator==(const Compact_class_driver &other) const { return idx == other.idx; }
    constexpr bool operator!=(const Compact_class_driver &other) const { return !(*this == other); }

    template <typename H>
    friend H AbslHashValue(H h, const Compact_class_driver &s) {
      return H::combine(std::move(h), s.idx);
    }
  };

  template <typename H>
  friend H AbslHashValue(H h, const Node_pin &s) {
    return H::combine(std::move(h), s.hidx.get_hash(), (int)s.idx, s.sink);  // Ignore lgraph pointer in hash
  }

  constexpr Node_pin() : top_g(0), current_g(0), idx(0), pid(0), sink(false) {}
  // rest can not be constexpr (find pid)
  Node_pin(LGraph *_g, Compact comp);
  Node_pin(LGraph *_g, Compact_driver comp);
  Node_pin(LGraph *_g, Compact_class comp);
  Node_pin(LGraph *_g, const Hierarchy_index &hidx, Compact_class comp);
  Node_pin(LGraph *_g, Compact_class_driver comp);

  // No constexpr (get_root_idx)
  Compact        get_compact() const {
    if(hidx.is_invalid())
      return Compact(Hierarchy_tree::root_index(), get_root_idx(), sink);
    return Compact(hidx, get_root_idx(), sink);
  }
  Compact_driver get_compact_driver() const;
  Compact_class  get_compact_class() const {
    // OK to pick a hierarchical to avoid replication of info like names
    return Compact_class(idx, sink);
  }

  Compact_class_driver get_compact_class_driver() const {
    // OK to pick a hierarchical to avoid replication of info like names
    I(!sink);  // Only driver pin allowed
    return Compact_class_driver(get_root_idx());
  }

  LGraph *        get_top_lgraph() const { return top_g; };
  LGraph *        get_class_lgraph() const { return current_g; };
  Hierarchy_index get_hidx() const { return hidx; };

  constexpr Port_ID get_pid() const { return pid; }

  bool has_inputs() const;
  bool has_outputs() const;

  bool is_graph_io() const;
  bool is_graph_input() const;
  bool is_graph_output() const;

  Node_pin get_driver() const;
  Node_pin get_sink() const;
  Node_pin get_sink_from_output() const {
    I(is_graph_output());
    return get_sink();
  }

  Node_pin get_driver_from_output() const {
    I(is_graph_output());
    return get_driver();
  }

  bool is_sink() const {
    I(idx);
    return sink;
  }
  bool is_driver() const {
    I(idx);
    return !sink;
  }

  Node     get_node() const;

  Node              get_driver_node() const; // common 0 or 1 driver case
  Node_pin          get_driver_pin() const;  // common 0 or 1 driver case
  Node_pin_iterator inp_driver() const; // handle 0 to inf driver case

  bool del_driver(Node_pin &dst);
  bool del_sink(Node_pin &dst);
  bool del(Node_pin &dst) {
    if (dst.is_sink() && is_driver())
      return del_sink(dst);
    I(dst.is_driver() && is_sink());
    return del_driver(dst);
  }

  void connect_sink(const Node_pin &dst) const;
  void connect_sink(const Node &dst) const;
  void connect_driver(const Node_pin &dst) const;
  void connect_driver(const Node &dst) const;
  void connect(const Node_pin &dst) const {
    if (dst.is_sink() && is_driver())
      return connect_sink(dst);
    I(dst.is_driver() && is_sink());
    return connect_driver(dst);
  }
  int get_num_edges() const;

  // NOTE: No operator<() needed for std::set std::map to avoid their use. Use flat_map_set for speed

  void           invalidate() { idx = 0; }
  constexpr bool is_invalid() const { return idx == 0; }
  constexpr bool is_down_node() const { return top_g != current_g; }
  constexpr bool is_hierarchical() const { return !hidx.is_invalid(); }
  Node_pin       get_non_hierarchical() const;

  bool operator==(const Node_pin &other) const {
    GI(idx == 0, hidx.is_invalid());
    GI(other.idx == 0, other.hidx.is_invalid());
    GI(idx && other.idx, top_g == other.top_g);
    return get_root_idx() == other.get_root_idx()
           && sink == other.sink
           && (hidx == other.hidx || hidx.is_invalid() || other.hidx.is_invalid());
  }
  bool operator!=(const Node_pin &other) const { return !(*this == other); }

  void nuke();  // Delete all the edges, and attributes of this node_pin

  // BEGIN ATTRIBUTE ACCESSORS
  std::string debug_name() const;

  void             set_name(std::string_view wname);
  void             erase_name();
  std::string_view get_name() const;
  bool             has_name() const;
  static Node_pin  find_driver_pin(LGraph *top, std::string_view wname);
  std::string_view get_pin_name() const;

  void             set_prp_vname(std::string_view prp_vname);
  std::string_view get_prp_vname() const;
  bool             has_prp_vname() const;
  void             dump_all_prp_vname() const;

  void  set_delay(float val);
  float get_delay() const;

  uint32_t get_bits() const;
  void     set_bits(uint32_t bits);

  std::string_view get_type_sub_io_name() const;
  std::string_view get_type_sub_pin_name() const;

  void   set_offset(Bits_t offset);
  Bits_t get_offset() const;

  const Ann_ssa &get_ssa() const;
  Ann_ssa *      ref_ssa();
  bool           has_ssa() const;
  bool           is_connected() const;
  bool           is_connected(const Node_pin &pin) const;

  // END ATTRIBUTE ACCESSORS
  XEdge_iterator out_edges() const;
  XEdge_iterator inp_edges() const;

  Node_pin get_down_pin() const;
  Node_pin get_up_pin() const;
};

namespace mmap_lib {
template <>
struct hash<Node_pin::Compact> {
  size_t operator()(Node_pin::Compact const &o) const {
    uint64_t h = o.idx;
    h          = (h << 12) ^ o.hidx.get_hash() ^ o.idx;
    return hash<uint64_t>{}((h << 1) + o.sink);
  }
};

template <>
struct hash<Node_pin::Compact_driver> {
  size_t operator()(Node_pin::Compact_driver const &o) const {
    uint64_t h = o.idx;
    h          = (h << 12) ^ o.hidx.get_hash() ^ o.idx;
    return hash<uint64_t>{}(h);
  }
};

template <>
struct hash<Node_pin::Compact_class> {
  size_t operator()(Node_pin::Compact_class const &o) const {
    uint32_t h = o.idx;
    return hash<uint32_t>{}((h << 1) + o.sink);
  }
};

template <>
struct hash<Node_pin::Compact_class_driver> {
  size_t operator()(Node_pin::Compact_class_driver const &o) const { return hash<uint32_t>{}(o.idx); }
};
}  // namespace mmap_lib
