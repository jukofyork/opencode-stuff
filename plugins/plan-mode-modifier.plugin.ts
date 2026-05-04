import type { Plugin } from "@opencode-ai/plugin";

export const PlanModeModifier: Plugin = async () => {
  return {
    "experimental.chat.messages.transform": async (input, output) => {
      output.messages = output.messages.map((message) => {
        message.parts = message.parts.map((part) => {
          if (
            part.type === "text" &&
            part.text.includes("# Plan Mode - System Reminder")
          ) {
            return {
              ...part,
              text: part.text.replace(
                "ZERO exceptions.",
                "The ONLY exception is creating or editing Markdown plan\nfiles (e.g. PLAN.md), but ONLY if the user EXPLICITLY requests one to be written."
              )
            };
          }
          return part;
        });
        return message;
      });
    },
  };
};
