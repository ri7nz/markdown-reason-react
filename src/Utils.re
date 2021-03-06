open Types;

exception MalformedMarkdown(string);

let rec getIndent = (count, str) =>
  switch (str) {
  | "" => count
  | " " => getIndent(count + 1, "")
  | _ =>
    switch (String.index(str, ' ')) {
    | i =>
      i != 0 ?
        getIndent(count, "") :
        getIndent(count + 1, String.sub(str, 1, String.length(str) - 1))
    | exception Not_found => getIndent(count, "")
    }
  };

let primitiveToString = t =>
  switch (t) {
  | Paragraph => "Paragraph"
  | Blockquote => "Blockquote"
  | Heading(int) => "Heading(" ++ string_of_int(int) ++ ")"
  | Code => "Code"
  | InlineCode => "InlineCode"
  | List(Ordered) => "List(Ordered)"
  | List(Unordered) => "List(Unordered)"
  | ListItem => "ListItem"
  | Table => "Table"
  | TableRow => "TableRow"
  | TableCell => "TableCell"
  | ThematicBreak => "ThematicBreak"
  | Break => "Break"
  | Emphasis => "Emphasis"
  | Strong => "Strong"
  | Delete => "Delete"
  | Link => "Link"
  | Image => "Image"
  | Footnote => "Footnote"
  | LinkReference => "LinkReference"
  | ImageReference => "ImageReference"
  | FootnoteReference => "FootnoteReference"
  | Definition => "Definition"
  | FootnoteDefinition => "FootnoteDefinition"
  | Null => "Null"
  };

let strIdx = (prmtv, (sl, el), str, ch, fail) =>
  switch (String.index(str, ch)) {
  | i => Some(i)
  | exception Not_found =>
    fail ?
      raise(
        MalformedMarkdown(
          "The "
          ++ prmtv
          ++ " at ("
          ++ string_of_int(sl)
          ++ ", "
          ++ string_of_int(el)
          ++ ") "
          ++ "is malformed.",
        ),
      ) :
      None
  | exception (Invalid_argument(_c)) =>
    fail ?
      failwith(
        "MarkdownReasonReact.Parser.safeStringindex: This is a bug, please report it.",
      ) :
      None
  };

let strIdxFrom = (prmtv, (sl, el), str, index, ch, fail) =>
  switch (String.index_from(str, index, ch)) {
  | i => Some(i)
  | exception Not_found =>
    fail ?
      raise(
        MalformedMarkdown(
          "The "
          ++ prmtv
          ++ " at ("
          ++ string_of_int(sl)
          ++ ", "
          ++ string_of_int(el)
          ++ ") "
          ++ "is malformed.",
        ),
      ) :
      None
  | exception (Invalid_argument(_c)) =>
    fail ?
      failwith(
        "MarkdownReasonReact.Parser.safeStringindexFrom: This is a bug, please report it.",
      ) :
      None
  };

let strSub = (_prmtv, (_sl, _el), str, ss, se, fail) =>
  switch (String.sub(str, ss, se)) {
  | i => Some(i)
  | exception (Invalid_argument(_c)) =>
    fail ?
      failwith(
        "MarkdownReasonReact.Parser.safeStringSub: This is a bug, please report it.",
      ) :
      None
  };

let addSpace = depth => {
  let x = ref(0);
  let spaces = ref("");
  while (x^ < depth) {
    spaces := spaces^ ++ " ";
    x := x^ + 1;
  };
  spaces^;
};

let hasTextContent = tc =>
  switch (tc) {
  | Some(s) => s
  | None => "None"
  };

let nestRuleToStr = nestRule =>
  List.fold_left(
    (acc, curr) => acc ++ primitiveToString(curr) ++ ", ",
    "[",
    nestRule,
  )
  ++ "]";

let nodeToString = (element: element) =>
  addSpace(0)
  ++ "{\n"
  ++ addSpace(2)
  ++ "element: "
  ++ primitiveToString(element.element)
  ++ ",\n"
  ++ addSpace(2)
  ++ "startLoc: "
  ++ string_of_int(element.startLoc)
  ++ ",\n"
  ++ addSpace(2)
  ++ "endLoc: "
  ++ string_of_int(element.endLoc)
  ++ ",\n"
  ++ addSpace(2)
  ++ "nestRule: "
  ++ nestRuleToStr(element.nestRule)
  ++ ",\n"
  ++ addSpace(2)
  ++ "children: []"
  ++ ",\n"
  ++ addSpace(0 + 2)
  ++ "textContent: "
  ++ hasTextContent(element.textContent)
  ++ ",\n"
  ++ addSpace(0)
  ++ "}";

let codegenToString = (_file, page) =>
  Lexing.from_string(page)
  |> Reason_toolchain.RE.implementation_with_comments
  |> Reason_toolchain.RE.print_implementation_with_comments(
       Format.str_formatter,
     )
  |> Format.flush_str_formatter;

let cleanText = node =>
  switch (node.textContent) {
  | Some(s) =>
    let trimmedStr = String.trim(s);

    let next =
      switch (node.element) {
      | Heading(1) =>
        String.sub(trimmedStr, 2, String.length(trimmedStr) - 2)
      | Heading(2) =>
        String.sub(trimmedStr, 3, String.length(trimmedStr) - 3)
      | Heading(3) =>
        String.sub(trimmedStr, 4, String.length(trimmedStr) - 4)
      | Heading(4) =>
        String.sub(trimmedStr, 5, String.length(trimmedStr) - 5)
      | Heading(5) =>
        String.sub(trimmedStr, 6, String.length(trimmedStr) - 6)
      | Heading(_) =>
        String.sub(trimmedStr, 7, String.length(trimmedStr) - 7)
      | Blockquote =>
        String.sub(trimmedStr, 2, String.length(trimmedStr) - 2)
      | Paragraph => trimmedStr
      | Break => trimmedStr
      | Emphasis => String.sub(trimmedStr, 1, String.length(trimmedStr) - 2)
      | Strong => String.sub(trimmedStr, 2, String.length(trimmedStr) - 3)
      | Delete => String.sub(trimmedStr, 2, String.length(trimmedStr) - 3)
      | List(_listTypes) => trimmedStr
      | ListItem => String.sub(trimmedStr, 2, String.length(trimmedStr) - 3)
      | Code => String.sub(trimmedStr, 3, String.length(trimmedStr) - 3)
      | InlineCode => trimmedStr
      | Table => trimmedStr
      | TableRow => trimmedStr
      | TableCell => trimmedStr
      | ThematicBreak => trimmedStr
      | Link => trimmedStr
      | Image => trimmedStr
      | Footnote => trimmedStr
      | LinkReference => trimmedStr
      | ImageReference => trimmedStr
      | FootnoteReference => trimmedStr
      | Definition => trimmedStr
      | FootnoteDefinition => trimmedStr
      | Null => ""
      };
    {...node, textContent: Some(next)};
  | None => node
  };