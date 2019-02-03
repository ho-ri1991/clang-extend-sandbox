#ifndef LLVM_CLANG_PARSE_EXTEND_PARSER_H
#define LLVM_CLANG_PARSE_EXTEND_PARSER_H

#include "clang/Parse/Parser.h"

namespace clang {

class ExtendParser : public Parser {
public:
  struct MemberVariableToken
  {
    CachedTokens TypenameTokens;
    Token NameToken;
    CachedTokens InitializerTokens;
    AccessSpecifier AS;
  };
  struct FunctionParamToken
  {
    CachedTokens TypenameTokens;
    Token NameToken;
    CachedTokens DefaultTokens;
  };
  struct MemberFunctionToken
  {
    CachedTokens TypenameTokens;
    Token NameToken;
    std::vector<FunctionParamToken> Params;
    CachedTokens QualifierTokens;
    CachedTokens BodyTokens;
    AccessSpecifier AS;
  };

  ExtendParser(Preprocessor &PP, Sema &Actions, bool SkipFunctionBodies);
  ~ExtendParser() override;

  StmtResult
  ParseStatementOrDeclaration(StmtVector &Stmts, AllowedConstructsKind Allowed,
                              SourceLocation *TrailingElseLoc = nullptr) override;

  ExprResult ParseAssignmentExpression(TypeCastState isTypeCast = NotTypeCast) override;

  ExprResult
  ParseExpression(TypeCastState isTypeCast = NotTypeCast) override;

  bool ParseTopLevelDecl(DeclGroupPtrTy &Result) override;

public:
  DeclGroupPtrTy ParseCXXClassMemberDeclaration(
      AccessSpecifier AS, AttributeList *Attr,
      const ParsedTemplateInfo &TemplateInfo = ParsedTemplateInfo(),
      ParsingDeclRAIIObject *DiagsFromTParams = nullptr) override;
private:

  void ParseDeclarationSpecifiers(
      DeclSpec &DS,
      const ParsedTemplateInfo &TemplateInfo = ParsedTemplateInfo(),
      AccessSpecifier AS = AS_none,
      DeclSpecContext DSC = DeclSpecContext::DSC_normal,
      LateParsedAttrList *LateAttrs = nullptr,
      Expr* MetaCall = nullptr) override;

  ExprResult ParseClassMemberAndGenerateMetaFunctionCallExpr(const CachedTokens& qualifiedMetaFunction, AccessSpecifier AS);

  bool ConsumeAndStoreTypename(CachedTokens& Result);

  std::string GenerateMetaFunctionCallExpr(
      const std::vector<MemberVariableToken> MemberVariables,
      const std::vector<MemberFunctionToken> MemberFunctions,
      const CachedTokens& QualifiedMetaFunction
  );

  void AppendToken(const Token& Tok, std::string& Target);
                   
  void AppendTokenStr(const Token& Source, std::string& Dest);
};

}

#endif
