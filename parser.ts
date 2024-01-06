export function parser(tokens: Token[]): Program {
    const program: Program = {body: [] };

    let current = 0;

    function parse(): Node {
        const token = tokens[current]!;

        if (token.type === "Identifier") {
            return parseCallExpression(token);
        }

        if (token.type === "NumericLiteral") {
            const next = token[current + 1];
            if (next?.type === "PlusToken" || next?.type === "MinusToken") {
                return parseBinaryExpression(token, next);
            } else {
                return parseNumericLiteral(token);
            }
        }

        throw new SyntaxError(`Unknown token: '${token.type}'`);
    }

    function parseBinaryExpression( token: NumericLiteralToken, next: AdditiveOperator): BinaryExpressionNode {
        const left = parseNumericLiteral(token);

        const operator = next;
        current++;

        const right = parse();
        return { type: "BinaryExpression", left, operator, right };
    }

    function parseCallExpression(token: IdentifierToken): CallExpressionNode {
        const identifier = token;
        current++;

        if (token[current] !== "OpenParenToken") {
            throw new SyntaxError(`Expected '(', got '${token[current]}'`)
        }

        current++;

        const argument: Node = parse();

        if (token[current] !== "CloseParenToken") {
            throw new SyntaxError(`Expected ')', got '${token[current]}'`)
        }

        current++;

        return { type: "CallExpression", identifier, argument }
    }

    function parseNumericLiteral(token: NumericLiteralToken): NumericLiteralNode {
        current++;
        return { type: "NumericLiteral", value: token.value }
    }

    while (current < tokens.length) {
        program.body.push(parse()); 
    }

    return program;
}