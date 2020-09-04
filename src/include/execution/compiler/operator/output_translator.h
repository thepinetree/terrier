#pragma once

#include <string_view>
#include <vector>

#include "execution/compiler/operator/operator_translator.h"
#include "execution/compiler/pipeline.h"

namespace terrier::parser {
class AbstractExpression;
}  // namespace terrier::parser

namespace terrier::execution::compiler {

class FunctionBuilder;

/**
 * A translator for outputting tuples.
 */
class OutputTranslator : public OperatorTranslator {
 public:
  /**
   * Create a translator for the given plan.
   * @param plan The plan.
   * @param compilation_context The context this translator belongs to.
   * @param pipeline The pipeline this translator is participating in.
   */
  OutputTranslator(const planner::AbstractPlanNode &plan, CompilationContext *compilation_context, Pipeline *pipeline);

  /**
   * This class cannot be copied or moved.
   */
  DISALLOW_COPY_AND_MOVE(OutputTranslator);

  /**
   * Tear-down the FilterManager if required.
   * @param pipeline The current pipeline.
   * @param function The pipeline generating function.
   */
  void TearDownPipelineState(const Pipeline &pipeline, FunctionBuilder *function) const override;

  /**
   * Define the output struct.
   */
  void DefineHelperStructs(util::RegionVector<ast::StructDecl *> *decls) override;

  /**
   * @param pipeline The current pipeline.
   * @param function The pipeline generating function.
   */
  void InitializePipelineState(const Pipeline &pipeline, FunctionBuilder *function) const override;

  /**
   * Perform the main work of the translator.
   */
  void PerformPipelineWork(WorkContext *context, FunctionBuilder *function) const override;

  /**
   * Does not interact with tables.
   */
  ast::Expr *GetTableColumn(catalog::col_oid_t col_oid) const override {
    UNREACHABLE("Output does not interact with tables.");
  }

 private:
  ast::Identifier output_var_;
  ast::Identifier output_struct_;

  // The number of rows that are output.
  StateDescriptor::Entry num_output_;

  // The OutputBuffer to use
  StateDescriptor::Entry output_buffer_;
};

}  // namespace terrier::execution::compiler
