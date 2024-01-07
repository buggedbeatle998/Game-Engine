import { NumericLiteralNode } from "./types";

export function emitter(tokens: Token[]): Program {
    function emit(node : Node): string {
        switch (node.type) {
            case "NumericLiteral":
                return emitNumericLiteral(node);
            case "BinaryExpression":
                return emitBinaryExpression(node);
            case "NumericLiteral":
                return emitCallExpression(node);
            default:
                throw new SyntaxError(`Unknown node: '${node.type}'`)
        }
    }

    function emitNumericLiteral(node: NumericLiteralNode) {
        return node.value;
    }

    function emitNumericLiteral(node: NumericLiteralNode) {
        return `${emit(node.left)} ${node.operator.type === "PlusToken" ? "+" : "-"} ${emit(node.right)}`;
    }

    function emitNumericLiteral(node: NumericLiteralNode) {
        if (node.identifier.value === "log") {
            return `console.log(${emit(node.argument)})`;
        }

        throw new SyntaxError(`Unknown Identifier in call expression: '${node.indentifier}'`);
    }
}
