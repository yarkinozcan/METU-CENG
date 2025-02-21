import java.util.ArrayList;

public class ScholarNodeSecondaryIndex extends ScholarNode {
	private ArrayList<String> journals;
	private ArrayList<ScholarNode> children;

	public ScholarNodeSecondaryIndex(ScholarNode parent) {
		super(parent);
		journals = new ArrayList<String>();
		children = new ArrayList<ScholarNode>();
		this.type = ScholarNodeType.Internal;
	}

	public ScholarNodeSecondaryIndex(ScholarNode parent, ArrayList<String> journals, ArrayList<ScholarNode> children) {
		super(parent);
		this.journals = journals;
		this.children = children;
		this.type = ScholarNodeType.Internal;
	}
	
	// GUI Methods - Do not modify
	public ArrayList<ScholarNode> getAllChildren()
	{
		return this.children;
	}
	
	public ScholarNode getChildrenAt(Integer index) {

		return this.children.get(index);
	}

	public Integer journalCount()
	{
		return this.journals.size();
	}
	
	public String journalAtIndex(Integer index) {
		if(index >= this.journalCount() || index < 0) {
			return "Not Valid Index!!!";
		}
		else {
			return this.journals.get(index);
		}
	}

	public void addOnlyChildAtTheFront(ScholarNode child) {
		children.addFirst(child);
	}
	
	// Extra functions if needed

	public void addChild(String journal, ScholarNode newChild) {
		int insertIndex = 0;
		while (insertIndex < journals.size() && journals.get(insertIndex).compareTo(journal) < 0) {
			insertIndex++;
		}
		journals.add(insertIndex, journal);

		if (insertIndex + 1 > children.size()) {
			children.add(newChild);
		} else {
			children.add(insertIndex + 1, newChild);
		}

		newChild.setParent(this);
	}

	public void removeLastChild() {
		if (!children.isEmpty()) {
			children.removeLast();
		}
		if (!journals.isEmpty()) {
			journals.removeLast();
		}
	}

	public boolean isChildren(ScholarNode node) {
		for (int i = 0; i < this.journalCount() + 1; i++) {
			if (this.getChildrenAt(i) == node) {
				return true;
			}
		}
		return false;
	}

	public void addKeyAtIndex(String journal, int index) {
		this.journals.add(index, journal);
	}

	public ScholarNode splitSecondaryIndex(ScholarNodeSecondaryIndex indexNode) {
		ScholarNodeSecondaryIndex newIndexNode = new ScholarNodeSecondaryIndex(indexNode.getParent());

		int midIndex = journals.size() / 2;
		String splitKey = journals.get(midIndex);

		for (int i = midIndex + 1; i < journals.size(); i++) {
			newIndexNode.journals.add(journals.get(i));
		}
		for (int i = midIndex + 1; i <= children.size() - 1; i++) {
			newIndexNode.children.add(children.get(i));
			children.get(i).setParent(newIndexNode);
		}

		while (journals.size() > midIndex) {
			journals.removeLast();
		}
		while (children.size() > midIndex + 1) {
			children.removeLast();
		}

		if (indexNode.getParent() == null) {
			ArrayList<String> newRootKeys = new ArrayList<>();
			ArrayList<ScholarNode> newRootChildren = new ArrayList<>();
			newRootKeys.add(splitKey);
			newRootChildren.add(this);
			newRootChildren.add(newIndexNode);

			ScholarNodeSecondaryIndex newRoot = new ScholarNodeSecondaryIndex(null, newRootKeys, newRootChildren);
			this.setParent(newRoot);
			newIndexNode.setParent(newRoot);
			return newRoot;
		} else {
			ScholarNodeSecondaryIndex parent = (ScholarNodeSecondaryIndex) indexNode.getParent();
			parent.addChild(splitKey, newIndexNode);
			return parent;
		}
	}

	public int findChildIndexForJournal(String journal) {
		for (int i = 0; i < this.journalCount(); i++) {
			if (journal.compareTo(this.journalAtIndex(i)) < 0) {
				return i;
			}
		}
		return this.journalCount();
	}

	public ScholarNode splitSecondaryIndex() {
		int midIndex = this.journalCount() / 2;
		String splitKey = this.journalAtIndex(midIndex);

		ScholarNodeSecondaryIndex newIndexNode = new ScholarNodeSecondaryIndex(this.getParent());

		newIndexNode.addOnlyChildAtTheFront(this.getChildrenAt(midIndex + 1));

		for (int i = midIndex + 1; i < this.journalCount(); i++) {
			newIndexNode.addChild(this.journalAtIndex(i), this.getChildrenAt(i + 1));
		}

		while (this.journalCount() > midIndex) {
			this.removeLastChild();
		}

		if (this.getParent() == null) {
			ArrayList<String> newRootKeys = new ArrayList<>();
			ArrayList<ScholarNode> newRootChildren = new ArrayList<>();
			newRootKeys.add(splitKey);
			newRootChildren.add(this);
			newRootChildren.add(newIndexNode);

			ScholarNodeSecondaryIndex newRoot = new ScholarNodeSecondaryIndex(null, newRootKeys, newRootChildren);
			newIndexNode.level = 1;
			this.level = 1;
			this.setParent(newRoot);
			newIndexNode.setParent(newRoot);
			return newRoot;
		} else {
			ScholarNodeSecondaryIndex parent = (ScholarNodeSecondaryIndex) this.getParent();
			int splitKeyIndex = parent.findChildIndexForJournal(splitKey);
			parent.addKeyAtIndex(splitKey, splitKeyIndex);
			parent.getAllChildren().add(splitKeyIndex + 1, newIndexNode);
			newIndexNode.setParent(parent);

			if (parent.journalCount() > 2 * ScholarNode.order) {
				return splitSecondaryIndex(parent);
			}

			return parent;
		}
	}
}
