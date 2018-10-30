
#include <unistd.h>

#include "eprp_utils.hpp"
#include "lgbench.hpp"

#include "cops_live_api.hpp"
#include "diff_finder.hpp"
#include "invariant_finder.hpp"
#include "invariant_options.hpp"
#include "live_options.hpp"
#include "structural.hpp"
#include "stitcher.hpp"
#include "stitch_options.hpp"

void Cops_live_api::invariant_finder(Eprp_var &var) {

  LGBench b;
  Invariant_find_options pack;
  for(const auto &l:var.dict) {
    pack.set(l.first,l.second);
  }

  Invariant_finder     worker(pack);
  b.sample("cops.live.inv_finder.setup");

  Invariant_boundaries fibs = worker.get_boundaries();
  b.sample("cops.live.inv_finder.work");

  std::ofstream of(pack.invariant_file);
  Invariant_boundaries::serialize(&fibs, of);
  of.close();
  b.sample("cops.live.inv_finder.output");
}

void Cops_live_api::diff_finder(Eprp_var &var) {
  LGBench b;

  Live_pass_options pack;
  for(const auto &l:var.dict) {
    pack.set(l.first,l.second);
  }

  Diff_finder      worker(pack);
  b.sample("cops.live.diff.setup");

  std::set<Net_ID> diffs;
  worker.generate_delta(pack.modified_lgdb, pack.delta_lgdb, diffs);

  b.sample("cops.live.diff.find_diffs");

  std::ofstream of(pack.diff_file);
  for(auto &diff_ : diffs) {
    of << diff_.first << "\t" << diff_.second << std::endl;
  }
  of.close();

  b.sample("cops.live.diff.write_diff");
}

void Cops_live_api::netlist_merge(Eprp_var &var) {
  LGBench b;

  Stitch_pass_options pack;
  for(const auto &l:var.dict) {
    pack.set(l.first,l.second);
  }

  if(pack.method == Stitch_pass_options::Live_method::LiveSynth) {
    Live_stitcher worker(pack);
    b.sample("cops.live.stitch.setup");

    std::ifstream    ifs(pack.diff_file);
    std::set<Net_ID> diffs;
    WireName_ID      net;
    uint32_t         bit;

    while(ifs >> net >> bit) {
      diffs.insert(std::make_pair(net, bit));
    }
    ifs.close();

    b.sample("cops.live.stitch.read_diff");
    worker.stitch(pack.nsynth_lgdb, diffs);

  } else if(pack.method == Stitch_pass_options::Live_method::Structural) {
    Live_structural worker(pack);
    b.sample("cops.live.stitch.setup");
    worker.replace(pack.nsynth_lgdb);

  } else {
    console->error("Unsupported stitch method {}\n", pack.method);
    return;
  }

  b.sample("cops.live.stitch.done");
}

void Cops_live_api::setup(Eprp &eprp) {
  Eprp_method inv_find("live.invariant_find", "find invariant boundaries between post-synthesis and post-elaboration lgraphs", &Cops_live_api::invariant_finder);
  inv_find.add_label_required("top", "top module");
  inv_find.add_label_required("elab_lgdb","lgdb path of the elaborated netlist");
  inv_find.add_label_required("synth_lgdb","lgdb path of the synthesized netlist");
  inv_find.add_label_required("invariant_file","file to serialize the invariant boundaries object (used by diff)");
  inv_find.add_label_required("hier_sep","hierarchical separator used in names by the synthesis tool");

  eprp.register_method(inv_find);

  Eprp_method diff_find("live.diff_finder", "find cones that changed between two post-elaboration lgraphs", &Cops_live_api::diff_finder);
  diff_find.add_label_required("olgdb","original elaborated lgdb path");
  diff_find.add_label_required("mlgdb","modified elaborated lgdb path");
  diff_find.add_label_required("slgdb","original synthesized lgdb path");
  diff_find.add_label_required("invariant_file","file to serialize the invariant boundaries object (generated by invariant finder)");
  diff_find.add_label_required("dlgdb","lgdb path for the delta netlist");
  diff_find.add_label_required("diff_file","output diff_file (used by the stitch pass)");
  eprp.register_method(diff_find);

  Eprp_method stitch("live.merge_changes", "merge synthesized delta into the original synthesized netlist", &Cops_live_api::netlist_merge);
  stitch.add_label_required("osynth","lgdb path for the original synthesized netlist");
  stitch.add_label_required("nsynth","lgdb path for the delta synthesized netlist");
  stitch.add_label_required("method","stitch method (options: LiveSynth and Structural");
  stitch.add_label_required("diff_file","input diff_file (generated by the diff pass)");
  eprp.register_method(stitch);
}
