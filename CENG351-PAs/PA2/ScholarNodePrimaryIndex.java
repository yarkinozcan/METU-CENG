import java.util.ArrayList;

public class ScholarNodePrimaryIndex extends ScholarNode {
	private ArrayList<Integer> paperIds;
	private ArrayList<ScholarNode> children;

	public ScholarNodePrimaryIndex(ScholarNode parent) {
		super(parent);
		paperIds = new ArrayList<Integer>();
		children = new ArrayList<ScholarNode>();
		this.type = ScholarNodeType.Internal;
	}

	public ScholarNodePrimaryIndex(ScholarNode parent, ArrayList<Integer> paperIds, ArrayList<ScholarNode> children) {
		super(parent);
		this.paperIds = paperIds;
		this.children = children;
		this.type = ScholarNodeType.Internal;
	}

	// GUI Methods - Do not modify
	public ArrayList<ScholarNode> getAllChildren()
	{
		return this.children;
	}

	public ScholarNode getChildrenAt(Integer index) {return this.children.get(index); }

	public Integer paperIdCount()
	{
		return this.paperIds.size();
	}

	public Integer childrenCount() {
		return this.children.size();
	}

	public Integer paperIdAtIndex(Integer index) {
		if(index >= this.paperIdCount() || index < 0) {
			return -1;
		}
		else {
			return this.paperIds.get(index);
		}
	}

	public void addOnlyChildAtTheFront(ScholarNode child) {
		children.addFirst(child);
	}

	public void addChild(Integer paperId, ScholarNode child) {
		int insertIndex = 0;
		while (insertIndex < paperIds.size() && paperIds.get(insertIndex) < paperId) {
			insertIndex++;
		}

		paperIds.add(insertIndex, paperId);

		if (insertIndex + 1 > children.size()) {
			children.add(child);
		} else {
			children.add(insertIndex + 1, child);
		}

		child.setParent(this);
	}

	public void removeLastChild() {
		if (!children.isEmpty()) {
			children.removeLast();
		}
		if (!paperIds.isEmpty()) {
			paperIds.removeLast();
		}
	}

	public int findChildIndexForPaper(Integer paperId) {
		for (int i = 0; i < this.paperIdCount(); i++) {
			if (paperId < this.paperIdAtIndex(i)) {
				return i;
			}
		}
		return this.paperIdCount();
	}

	public void addKeyAtIndex(Integer key, int index) {
		this.paperIds.add(index, key);
	}

	public boolean isChildren(ScholarNode node) {
		for (int i = 0; i < this.childrenCount(); i++) {
			if (this.getChildrenAt(i) == node) {
				return true;
			}
		}
		return false;
	}

	public ScholarNode splitPrimaryIndex() {
		int midIndex = this.paperIdCount() / 2;
		Integer splitKey = this.paperIdAtIndex(midIndex);

		ScholarNodePrimaryIndex newIndexNode = new ScholarNodePrimaryIndex(this.getParent());

		newIndexNode.addOnlyChildAtTheFront(this.getChildrenAt(midIndex + 1));

		for (int i = midIndex + 1; i < this.paperIdCount(); i++) {
			newIndexNode.addChild(this.paperIdAtIndex(i), this.getChildrenAt(i + 1));
		}

		while (this.paperIdCount() > midIndex) {
			this.removeLastChild();
		}

		if (this.getParent() == null) {
			ArrayList<Integer> newRootKeys = new ArrayList<>();
			ArrayList<ScholarNode> newRootChildren = new ArrayList<>();
			newRootKeys.add(splitKey);
			newRootChildren.add(this);
			newRootChildren.add(newIndexNode);

			ScholarNodePrimaryIndex newRoot = new ScholarNodePrimaryIndex(null, newRootKeys, newRootChildren);
			newIndexNode.level = 1;
			this.level = 1;
			this.setParent(newRoot);
			newIndexNode.setParent(newRoot);
			return newRoot;
		} else {
			ScholarNodePrimaryIndex parent = (ScholarNodePrimaryIndex) this.getParent();
			int splitKeyIndex = parent.findChildIndexForPaper(splitKey);
			parent.addKeyAtIndex(splitKey, splitKeyIndex);
			parent.getAllChildren().add(splitKeyIndex + 1, newIndexNode);
			newIndexNode.setParent(parent);
			newIndexNode.level = this.level + 1;
			this.level = this.level + 1;

			if (parent.paperIdCount() > 2 * ScholarNode.order) {
				return parent.splitPrimaryIndex();
			}

			return parent;
		}
	}
}
