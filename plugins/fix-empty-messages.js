/**
 * FixEmptyAssistantMessages Plugin
 * 
 * PROBLEM:
 * Moonshot AI (and some other providers) reject API calls with empty assistant messages,
 * returning: "Invalid request: the message at position X with role 'assistant' must not be empty"
 * 
 * ATTEMPTED SOLUTIONS THAT FAILED:
 * 1. Plugin hooks (message.updated, message.part.updated, event) - These operate at the 
 *    session/database layer, but the empty messages are created AFTER hooks fire and 
 *    BEFORE the HTTP request is sent to the provider. The plugin system doesn't expose 
 *    the provider transform layer where filtering needs to happen.
 * 
 * 2. Removing ALL empty assistant messages - This broke tool call chains because 
 *    subsequent "tool" role messages reference the tool_call_id from the assistant 
 *    messages. Removing them caused "tool_call_id is not found" errors.
 * 
 * SOLUTION:
 * Monkey-patch global.fetch to intercept the API request body and filter messages 
 * at the HTTP layer. This is the only layer where we can see the final message array 
 * being sent to the provider.
 * 
 * KEY INSIGHT:
 * Empty content (null/undefined/"") is acceptable to Moonshot IF the assistant message 
 * has tool_calls. Only messages with BOTH empty content AND no tool_calls need to be 
 * removed - these are "orphaned" empty messages that serve no purpose.
 */

export const FixEmptyAssistantMessages = async () => {      
  const originalFetch = globalThis.fetch;
  
  globalThis.fetch = async (url, options) => {
    const urlStr = typeof url === 'string' ? url : url.toString();
    
    if (urlStr.includes('openrouter') && options?.body) {
      try {
        const body = JSON.parse(options.body);
        
        if (body.messages) {
          const before = body.messages.length;
          
          body.messages = body.messages.filter(msg => {
            // Only remove empty assistant messages WITHOUT tool_calls
            if (msg.role !== 'assistant') return true;
            
            const content = msg.content;
            const hasToolCalls = msg.tool_calls?.length > 0;
            
            const isEmpty = 
              !content || 
              (typeof content === 'string' && content.trim() === '') ||
              (Array.isArray(content) && content.length === 0);
            
            // Keep if has tool_calls, remove only if truly empty/orphaned
            return !(isEmpty && !hasToolCalls);
          });
          
          if (body.messages.length !== before) {
            options = { ...options, body: JSON.stringify(body) };
          }
        }
      } catch (e) {}
    }
    
    return originalFetch(url, options);
  };
  
  return {};
};
