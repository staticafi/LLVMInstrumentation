#ifndef INSTR_PLUGIN_H
#define INSTR_PLUGIN_H

#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>

class InstrPlugin
{
	public:
      // the default behaviour is returning true, since we have
      // no information about the value, so it may be anything
	  virtual bool isNull(llvm::Value*) {
        return true;
      }

	  virtual bool isValidPointer(llvm::Value*) {
        return true;
      }

	  virtual bool isEqual(llvm::Value*, llvm::Value*) {
        return true;
      }

	  virtual bool isNotEqual(llvm::Value*, llvm::Value*) {
        return true;
      }

	  virtual bool greaterThan(llvm::Value*, llvm::Value*) {
        return true;
      }
	  virtual bool lessThan(llvm::Value*, llvm::Value*) {
        return true;
      }

	  virtual bool lessOrEqual(llvm::Value*, llvm::Value*) {
        return true;
      }

	  virtual bool greaterOrEqual(llvm::Value*, llvm::Value*) {
        return true;
      }

	  virtual bool isConstant(llvm::Value* a) {
			return llvm::isa<llvm::Constant>(a);
	  }

      // add virtual destructor, so that child classes will
      // call their destructor
      virtual ~InstrPlugin() {}
};

#endif
